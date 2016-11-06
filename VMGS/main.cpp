#include "global.hpp"

namespace VM76 {
	Shaders* main_shader = NULL;

	void control_update() {
		// Mouse Input
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		game_player.horizontal_angle -= 0.005 * (xpos - VMDE->width / 2.0);
		game_player.vertical_angle   -= 0.005 * (ypos - VMDE->height / 2.0);
		game_player.vertical_angle = glm::clamp(- PI / 2 + 0.04f, game_player.vertical_angle, PI / 2);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);
		glm::vec3 direction = glm::vec3(
			cos(game_player.vertical_angle) * sin(game_player.horizontal_angle),
			sin(game_player.vertical_angle),
			cos(game_player.vertical_angle) * cos(game_player.horizontal_angle)
		);
		glm::vec3 right = glm::vec3(
			sin(game_player.horizontal_angle - PI / 2.0f),
			0,
			cos(game_player.horizontal_angle - PI / 2.0f)
		);
		glm::vec3 up = glm::cross(right, direction);
		glm::vec3 cam_pos = game_player.wpos + glm::vec3(0.0, 1.68, 0.0);
		view = glm::lookAt(cam_pos, cam_pos + direction, up);

		// Key Input
		float speed = 3.0f / 60.0f;

		int state = glfwGetKey(window, game.forward);
		if (state == GLFW_PRESS) {
			game_player.wpos += glm::vec3(
				sin(game_player.horizontal_angle),
				0.0,
				cos(game_player.horizontal_angle)
			) * glm::vec3(speed);
			game_player.speed += 0.1;
		}
		state = glfwGetKey(window, game.back);
		if (state == GLFW_PRESS)
			game_player.wpos -= glm::vec3(
				sin(game_player.horizontal_angle),
				0.0,
				cos(game_player.horizontal_angle)
			) * glm::vec3(speed);
		state = glfwGetKey(window, game.left);
		if (state == GLFW_PRESS)
			game_player.wpos -= right * glm::vec3(speed);
		state = glfwGetKey(window, game.right);
		if (state == GLFW_PRESS)
			game_player.wpos += right * glm::vec3(speed);

	}

	void loop() {
		for (;;) {
			::main_draw_start();
			control_update();

			glClearColor(0.66f, 0.81f, 0.96f, 1.0f);
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnable(GL_BLEND);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			log("FPS %d",VMDE->fps);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDepthMask(GL_TRUE);
			main_shader->use();

			// Setup uniforms
			GLuint model_location = glGetUniformLocation(main_shader->shaderProgram, "brightness");
			glUniform1f(model_location, VMDE->state.brightness);

			// Setup textures
			char uniform_name[16];
			for (int index = 0; index < 16; index++) if (Res::tex_unit[index]) {
				sprintf(uniform_name, "colortex%d", index);
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, Res::tex_unit[index]->texture);
				glUniform1i(glGetUniformLocation(main_shader->shaderProgram, (GLchar*) uniform_name), index);
			}

			//float x = 55.4f * cos(0.005f * VMDE->frame_count);
			//float y = 55.4f * sin(0.005f * VMDE->frame_count);
			//view = glm::lookAt(glm::vec3(x, 6.0, y), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			main_shader->ProjectionView(projection, view);
			t[GRASS]->render();

			::main_draw_end();
		}
	}

	void start_game() {
		::init_engine(800, 600, "VM / 76");
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);

		new Res::Texture((char*)"../Media/terrain.png", 0);
		init_tiles();

		char* temp_vertexShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.vsh");
		char* temp_fragmentShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.fsh");
		main_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
		main_shader->link_program();

		projection = glm::perspective(1.3f, float(VMDE->width) / float(VMDE->height), 1.0f, -1.0f);
		view = glm::lookAt(glm::vec3(0.0, 2.6, 0.0), glm::vec3(1.0, 2.6, 0.0), glm::vec3(0.0, 1.0, 0.0));

		loop();
	}

	void init_tiles() {
		t[GRASS] = new Tile(0);
		t[STONE] = new Tile(1);
		t[DIRT] = new Tile(2);
		t[WOOD] = new Tile(4);
	}

	void terminate() {
		for (int i = 0; i < 16; i++) {
			t[i]->dispose();
			free(t[i]);
		}
	}
}

extern "C" {
	void client_terminate() {
		VM76::terminate();
	}
}

int main() {
	puts("Starting game");
	VM76::start_game();
}
