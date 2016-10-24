#include "global.hpp"

namespace VM76 {

	Tile::Tile(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx = new GLfloat[4 * 9 * 4] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,

			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,

			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,

			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
		};
		itx = new GLuint[3 * 6] {0,1,3, 1,2,3, 7,5,4, 7,6,5, 8,9,11, 9,10,11};
		obj = GDrawable::create();
		obj->vtx_c = 8 * 9;
		obj->ind_c = 7 * 3;
		obj->vertices = vtx;
		obj->indices = itx;
		obj->tri_mesh_count = 6 * 2;
		GDrawable::fbind(obj);
	}

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

			// Activate Shader
			glUseProgram(main_shader->shaderProgram);

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

			float x = 5.0f * cos(0.01f * VMDE->frame_count);
			float y = 5.0f * sin(0.01f * VMDE->frame_count);
			view = glm::lookAt(glm::vec3(x, 3.5f, y), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			//test_obj->model = glm::rotate(glm::mat4(1.0f), 0.01f * VMDE->frame_count, glm::vec3(0.0f, 1.0f, 0.0f));
			//for (int x = -2; x < 3; x++) {
			//	for (int y = -2; y < 3; y++) if (x != 0 && y != 0) {
//					t[3]->obj->model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f));
					GDrawable::prepare(t[3]->obj, projection, view);
					GDrawable::draw(t[3]->obj);
			//	}
			//}

			::main_draw_end();
		}
	}

	void start_game() {
		::init_engine(800, 600);

		new Res::Texture((char*)"../Media/terrain.png", 0);

		for (int i = 0; i < 16; i++) {
			t[i] = new Tile(i);
		}

		projection = glm::perspective(1.0f, float(VMDE->width) / float(VMDE->height), 1.0f, -1.0f);
		view = glm::lookAt(glm::vec3(2.0, 3.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		loop();
	}

	void terminate() {
		for (int i = 0; i < 16; i++) {
			GDrawable::dispose(t[i]->obj);
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
