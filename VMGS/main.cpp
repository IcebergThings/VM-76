#include "global.hpp"

namespace VM76 {
	Shaders* main_shader = NULL;
	Tile::Tile(int tid) {
		int x = tid % 16;
		int y = tid / 16;
		float T = 1.0f / 16.0f;
		float S = 0.0f;
		float xs = x * T;
		float ys = y * T;
		vtx = new GLfloat[6 * 9 * 4] {
			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,

			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,

			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,

			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,

			0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,

			1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+S,
			1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+S,
			1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  xs+T, ys+T,
			1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  xs+S, ys+T,
		};
		itx = new GLuint[3 * 12] {
			0,1,3,    1,2,3,    7,5,4,    7,6,5,
			8,9,11,   9,10,11,  15,13,12, 15,14,13,
			16,17,19, 17,18,19, 23,21,20, 23,22,21};

		mat = new glm::mat4[256 * 256];
		for (int x = 0; x < 256; x++) {
			for (int y = 0; y < 256; y++) {
				mat[x * 256 + y] = glm::translate(glm::mat4(1.0), glm::vec3(float(x) - 128.0f, 0.0, float(y) - 128.0f));
			}
		}

		obj = new GDrawable();
		obj->data.vtx_c = 6 * 9 * 4;
		obj->data.ind_c = 12 * 3;
		obj->data.vertices = vtx;
		obj->data.indices = itx;
		obj->data.tri_mesh_count = 6 * 2;
		obj->data.mat_c = 256 * 256;
		obj->data.mat = (GLuint*) &mat[0];
		obj->fbind();
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

			//test_obj->model = glm::rotate(glm::mat4(1.0f), 0.01f * VMDE->frame_count, glm::vec3(0.0f, 1.0f, 0.0f));
			//for (int x = -2; x < 3; x++) {
			//	for (int y = -2; y < 3; y++) if (x != 0 && y != 0) {
//					t[3]->obj->model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f));
					t[4]->obj->prepare(main_shader, projection, view);
					t[4]->obj->draw();
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

		temp_vertexShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.vsh");
		temp_fragmentShaderSource = Util::read_file("../Media/shaders/gbuffers_basic.fsh");
		main_shader = new Shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
		main_shader->link_program();

		projection = glm::perspective(1.0f, float(VMDE->width) / float(VMDE->height), 1.0f, -1.0f);
		view = glm::lookAt(glm::vec3(2.0, 3.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		loop();
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
