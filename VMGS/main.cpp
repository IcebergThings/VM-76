//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "global.hpp"

namespace VM76 {
	Shaders* main_shader = NULL;

	void update_fps() {
		char s[256];
		snprintf(s, 256, "VM / 76 - FPS: %d", VMDE->fps);
		glfwSetWindowTitle(window, s);
	}

	void control_update() {
		// Mouse Input
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		game_player.horizontal_angle -= 0.005 * (xpos - VMDE->width / 2.0);
		game_player.vertical_angle -= 0.005 * (ypos - VMDE->height / 2.0);
		game_player.vertical_angle = glm::clamp(
			- Util::PIf / 2 + 0.04f,
			game_player.vertical_angle,
			Util::PIf / 2
		);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);
		glm::vec3 direction = glm::vec3(
			cos(game_player.vertical_angle) * sin(game_player.horizontal_angle),
			sin(game_player.vertical_angle),
			cos(game_player.vertical_angle) * cos(game_player.horizontal_angle)
		);
		glm::vec3 right = glm::vec3(
			sin(game_player.horizontal_angle - Util::PIf / 2.0f),
			0,
			cos(game_player.horizontal_angle - Util::PIf / 2.0f)
		);
		glm::vec3 up = glm::cross(right, direction);
		glm::vec3 cam_pos = game_player.wpos + glm::vec3(0.0, 1.68, 0.0);
		view = glm::lookAt(cam_pos, cam_pos + direction, up);

		// Key Input
		glm::vec3 speed = glm::vec3(0.1);
		#define PRESS(key) glfwGetKey(window, game.key_##key) == GLFW_PRESS
		if (PRESS(quit)) VMDE->done = true;
		if (PRESS(forward))
			game_player.wpos += glm::vec3(direction.x, 0.0, direction.z) * speed;
		if (PRESS(back))
			game_player.wpos -= glm::vec3(direction.x, 0.0, direction.z) * speed;
		if (PRESS(left))
			game_player.wpos -= right * speed;
		if (PRESS(right))
			game_player.wpos += right * speed;
		if (PRESS(up))
			game_player.wpos += glm::vec3(0.0, 1.0, 0.0) * speed;
		if (PRESS(down))
			game_player.wpos -= glm::vec3(0.0, 1.0, 0.0) * speed;
		#undef PRESS
	}

	Structure* main_str;

	void loop() {
		for (;;) {
			::main_draw_start();
			control_update();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnable(GL_BLEND);
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			update_fps();
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

			main_shader->ProjectionView(projection, view);
			main_str->render();

			::main_draw_end();
			if (VMDE->done) break;
		}
	}

	void start_game() {
		// 先设好事件回调然后再启动引擎，最大地避免段错误
		on_terminate = terminate;
		::init_engine(860, 540, "VM / 76");
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);

		new Res::Texture("../Media/terrain.png", 0);

		main_str = new Structure();

		char* temp_vertexShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.vsh");
		char* temp_fragmentShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.fsh");
		main_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
		main_shader->link_program();

		projection = glm::perspective(1.3f, float(VMDE->width) / float(VMDE->height), 0.0f, -10.0f);
		view = glm::lookAt(glm::vec3(0.0, 2.6, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		loop();
	}

	void terminate() {
		main_str->dispose();
		free(main_str);
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
