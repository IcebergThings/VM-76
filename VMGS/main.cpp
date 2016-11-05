#include "global.hpp"

namespace VM76 {
	Shaders* main_shader = NULL;

	Tile* t[16];

	void loop() {
		for (;;) {
			::main_draw_start();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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

			float x = 55.4f * cos(0.005f * VMDE->frame_count);
			float y = 55.4f * sin(0.005f * VMDE->frame_count);
			view = glm::lookAt(glm::vec3(x, 6.0, y), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			t[GRASS]->obj->prepare(main_shader, projection, view);
			t[GRASS]->obj->draw();

			::main_draw_end();
		}
	}

	void start_game() {
		::init_engine(800, 600);

		new Res::Texture((char*)"../Media/terrain.png", 0);
		init_tiles();

		temp_vertexShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.vsh");
		temp_fragmentShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.fsh");
		main_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
		main_shader->link_program();

		projection = glm::perspective(1.0f, float(VMDE->width) / float(VMDE->height), 1.0f, -1.0f);
		view = glm::lookAt(glm::vec3(2.0, 3.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

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
			t[i]->obj->dispose();
			free(t[i]);
		}
	}
}

extern "C" {
	void client_terminate() {
		VM76::terminate();
	}

	void i_have_a_key(GLFWwindow* window UNUSED, int key UNUSED, int scancode UNUSED, int action UNUSED, int mode UNUSED) {
		log("Something happened");
	}
}

int main() {
	log("Starting game");
	VM76::start_game();
}
