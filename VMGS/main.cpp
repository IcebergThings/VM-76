//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {
	Shaders* shader_textured = NULL;
	Shaders* shader_basic = NULL;
	Res::Texture* tile_texture = NULL;

	Cube* block_pointer;
	TiledMap* map;

	Axis* axe;

	Object* obj = new Object();

	int map_count = 0;

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		#define PRESS(n) key == n && action == GLFW_PRESS
		if (PRESS(GLFW_KEY_A)) obj->move(glm::vec3(-0.5, 0.0, 0.0));
		if (PRESS(GLFW_KEY_D)) obj->move(glm::vec3(0.5, 0.0, 0.0));
		if (PRESS(GLFW_KEY_W)) obj->move(glm::vec3(0.0, 0.0, -0.5));
		if (PRESS(GLFW_KEY_S)) obj->move(glm::vec3(0.0, 0.0, 0.5));
		if (PRESS(GLFW_KEY_UP)) obj->move(glm::vec3(0.0, 0.5, 0.0));
		if (PRESS(GLFW_KEY_DOWN)) obj->move(glm::vec3(0.0, -0.5, 0.0));

		if (PRESS(GLFW_KEY_SPACE)) {
			Audio::play_sound("../Media/soft-ping.ogg", false);
			map->tiles[map->calcTileIndex(obj->pos * 2.0f)].tid = 2;
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

			shader_textured->ProjectionView(projection, view);
			map->render();


			// Setup uniforms
			shader_basic->use();
			shader_textured->ProjectionView(projection, view);
			block_pointer->mat[0] = obj->transform();
			block_pointer->update_instance(1);
			block_pointer->render();

			axe->render();

			::main_draw_end();
		} while (!VMDE->done);
	}

	void init_textures() {
		shader_textured->set_texture("colortex0", tile_texture, 0);
	}

	void start_game() {
		::init_engine(860, 540, "VM / 76");
		init_control();

		tile_texture = new Res::Texture("../Media/terrain.png");

		shader_textured = Shaders::CreateFromFile("../Media/shaders/gbuffers_textured.vsh", "../Media/shaders/gbuffers_textured.fsh");
		shader_basic = Shaders::CreateFromFile("../Media/shaders/gbuffers_basic.vsh", "../Media/shaders/gbuffers_basic.fsh");

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

		init_textures();

		block_pointer = new Cube(1);
		axe = new Axis();
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
		VMDE_Dispose(map);
		log("terminated successfully");
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
