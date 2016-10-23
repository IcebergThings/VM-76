#include <VMDE/VMDE.hpp>

namespace VM76 {
	GDrawable::GDrawable* test_obj;

	void loop() {
		for (;;) {
			::main_draw_start();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Activate Shader
			glUseProgram(main_shader->shaderProgram);

			// Setup uniforms
			GLuint model_location = glGetUniformLocation(main_shader->shaderProgram, "brightness");
			glUniform1f(model_location, VMDE->state.brightness);

			matrix2D();

			// Setup textures
			for (int index = 0; index < 16; index++) if (Res::tex_unit[index]) {
				char* uniform_name = new char[16];
				sprintf(uniform_name, "colortex%d", index);
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, Res::tex_unit[index]->texture);
				glUniform1i(glGetUniformLocation(main_shader->shaderProgram, (GLchar*) uniform_name), index);
				xefree(uniform_name);
			}

			::main_draw_end();
		}
	}

	void start_game() {
		::init_engine(800, 600);

		new Res::Texture((char*)"../Media/terrain.png", 0);
		test_obj = GDrawable::create();
//		GDrawable::fbind(test_obj);

		loop();
	}
}

int main() {
	log("Starting game");
	VM76::start_game();
}
