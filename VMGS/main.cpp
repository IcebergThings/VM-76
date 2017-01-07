//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   VMGS的主程序。
//=============================================================================

#include "VMGS.hpp"

namespace VM76 {
	Shaders* main_shader = NULL;

	Cube* c;

	void loop() {
		for (;;) {
			::main_draw_start();
			update_control();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			main_shader->use();

			// Setup uniforms
			GLuint model_location = glGetUniformLocation(main_shader->shaderProgram, "brightness");
			glUniform1f(model_location, VMDE->state.brightness);

			main_shader->ProjectionView(projection, view);
			c->render();

			::main_draw_end();
			if (VMDE->done) break;
		}
	}

	void update_textures() {
		char uniform_name[16];
		for (int index = 0; index < 16; index++) if (Res::tex_unit[index]) {
			sprintf(uniform_name, "colortex%d", index);
			glActiveTexture(GL_TEXTURE0 + index);
			glBindTexture(GL_TEXTURE_2D, Res::tex_unit[index]->texture);
			glUniform1i(glGetUniformLocation(main_shader->shaderProgram, (GLchar*) uniform_name), index);
		}
	}

	void start_game() {
		// 先设好事件回调然后再启动引擎，最大地避免段错误
		on_terminate = terminate;
		::init_engine(860, 540, "VM / 76");
		init_control();

		new Res::Texture("../Media/terrain.png", 0);

		char* temp_vertexShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.vsh");
		char* temp_fragmentShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.fsh");
		main_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
		main_shader->link_program();
		xefree(temp_vertexShaderSource);
		xefree(temp_fragmentShaderSource);

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

		update_textures();

		c = new Cube(1);

		loop();
	}

	void terminate() {
	}
}

int main() {
	log("Hello! This is VM76. Nice to meet you!");
	VM76::start_game();
}
