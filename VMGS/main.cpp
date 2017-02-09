//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {
	Shaders* shader_textured = NULL;
	Shaders* gui = NULL;
	Shaders* shader_basic = NULL;
	Res::Texture* tile_texture = NULL;

	TextRenderer* trex = NULL; // 2333 T-Rex

	Cube* block_pointer;
	Cube* clist[16];
	TiledMap* map;

	glm::mat4 gui_2d_projection;

	int hand_id = 1;

	Axis* axe;

	Object* obj = new Object();

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		#define PRESS(n) key == n && action == GLFW_PRESS
		if (PRESS(GLFW_KEY_A)) obj->move(glm::vec3(-1.0, 0.0, 0.0));
		if (PRESS(GLFW_KEY_D)) obj->move(glm::vec3(1.0, 0.0, 0.0));
		if (PRESS(GLFW_KEY_W)) obj->move(glm::vec3(0.0, 0.0, -1.0));
		if (PRESS(GLFW_KEY_S)) obj->move(glm::vec3(0.0, 0.0, 1.0));
		if (PRESS(GLFW_KEY_UP)) obj->move(glm::vec3(0.0, 1.0, 0.0));
		if (PRESS(GLFW_KEY_DOWN)) obj->move(glm::vec3(0.0, -1.0, 0.0));

		if (PRESS(GLFW_KEY_0)) hand_id = 0;
		if (PRESS(GLFW_KEY_0)) hand_id = 0;
		if (PRESS(GLFW_KEY_1)) hand_id = 1;
		if (PRESS(GLFW_KEY_2)) hand_id = 2;
		if (PRESS(GLFW_KEY_3)) hand_id = 3;
		if (PRESS(GLFW_KEY_4)) hand_id = 4;
		if (PRESS(GLFW_KEY_5)) hand_id = 5;
		if (PRESS(GLFW_KEY_6)) hand_id = 6;
		if (PRESS(GLFW_KEY_7)) hand_id = 7;
		if (PRESS(GLFW_KEY_8)) hand_id = 8;
		if (PRESS(GLFW_KEY_9)) hand_id = 9;

		if (PRESS(GLFW_KEY_SPACE)) {
			Audio::play_sound("../Media/soft-ping.ogg", false);
			map->tiles[map->calcTileIndex(obj->pos)].tid = hand_id;
			map->bake_tiles();
		}
		#undef PRESS
	}

	void loop() {
		do {
			::main_draw_start();
			update_control();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			shader_textured->use();

			// Setup uniforms
			shader_textured->set_float("brightness", VMDE->state.brightness);
			shader_textured->set_texture("colortex0", tile_texture, 0);

			// Textured blocks rendering
			shader_textured->ProjectionView(projection, view);
			map->render();

			// Setup uniforms
			// Non textured rendering
			shader_basic->use();
			shader_basic->set_float("opaque", 0.5);
			shader_textured->ProjectionView(projection, view);
			block_pointer->mat[0] = obj->transform();
			block_pointer->update_instance(1);
			block_pointer->render();

			axe->render();

			// GUI rendering
			gui->use();
			gui->set_texture("atlastex", tile_texture, 0);
			gui->ProjectionView(gui_2d_projection, glm::mat4(1.0));
			glDisable(GL_DEPTH_TEST);
			if (hand_id > 0)
				clist[hand_id - 1]->render();

			char frame_count[64];
			sprintf(frame_count, "FPS: %d Hand ID: %d Pointer ID: %d",
				VMDE->fps,
				hand_id,
				map->tiles[map->calcTileIndex(obj->pos)].tid
			);
			trex->instanceRenderText(
				frame_count, gui_2d_projection,
				glm::mat4(1.0),
				glm::translate(glm::mat4(1.0), glm::vec3(0.01,0.94,0.0)),
				0.025, 0.05, true
			);
			glEnable(GL_DEPTH_TEST);

			::main_draw_end();
		} while (!VMDE->done);
	}

	void start_game() {
		::init_engine(860, 540, "VM / 76");
		init_control();

		tile_texture = new Res::Texture("../Media/terrain.png");

		shader_textured = Shaders::CreateFromFile("../Media/shaders/gbuffers_textured.vsh", "../Media/shaders/gbuffers_textured.fsh");
		shader_basic = Shaders::CreateFromFile("../Media/shaders/gbuffers_basic.vsh", "../Media/shaders/gbuffers_basic.fsh");
		gui = Shaders::CreateFromFile("../Media/shaders/gui.vsh", "../Media/shaders/gui.fsh");
		float aspectRatio = float(VMDE->width) / float(VMDE->height);
		gui_2d_projection = glm::ortho(0.0, 1.0 * aspectRatio, 0.0, 1.0, -1.0, 1.0);

		projection = glm::perspective(1.3f, float(VMDE->width) / float(VMDE->height), 0.1f, 1000.0f);
		view = glm::lookAt(glm::vec3(0.0, 2.6, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		// GL settings initialize
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glDepthRange(0.0f, 1.0f);
		glClearDepth(1.0f);
		glDepthMask(GL_TRUE);

		block_pointer = new Cube(1);
		// Set up hand block indicator's matrix
		glm::mat4 block_display = glm::translate(glm::mat4(1.0), glm::vec3(0.02, 0.06, 0.2));
		block_display = glm::scale(block_display, glm::vec3(0.1f));
		block_display = glm::rotate(block_display, Util::PIf / 4.0f, glm::vec3(1.0, 0.0, 0.0));
		block_display = glm::rotate(block_display, Util::PIf / 4.0f, glm::vec3(0.0, 1.0, 0.0));
		for (int i = 0; i < 16; i++) {
			clist[i] = new Cube(i);
			clist[i]->mat[0] = block_display;
			clist[i]->update_instance(1);
		}
		axe = new Axis();
		trex = new TextRenderer();
		map = new TiledMap(16, 16, 16, glm::vec3(0.0));
		block_pointer->obj->data.mat_c = 1;
		glfwSetKeyCallback(window, key_callback);

		loop();
		terminate();
	}

	void terminate() {
		log("starting to terminate");
		terminate_engine();
		VMDE_Dispose(tile_texture);
		VMDE_Dispose(block_pointer);
		for (int i = 0; i < 16; i++) VMDE_Dispose(clist[i]);
		VMDE_Dispose(map);
		log("terminated successfully");
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
