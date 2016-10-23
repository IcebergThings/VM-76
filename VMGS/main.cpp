#include <VMDE/VMDE.hpp>

namespace VM76 {
	GDrawable::GDrawable* test_obj = NULL;

	void loop() {
		for (;;) {
			::main_draw_start();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClearDepth(1.0f);
			glClear(GL_DEPTH_BUFFER_BIT);
			glClear(GL_COLOR_BUFFER_BIT);

			glEnable(GL_BLEND);
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

			test_obj->model = glm::translate(test_obj->model, glm::vec3(0.0001f, 0.0001f, 0.0001f));
			test_obj->model = glm::rotate(test_obj->model, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
			GDrawable::draw(test_obj, projection, view);

			::main_draw_end();
		}
	}

	static GLfloat vtx[] = {
		0.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,
		0.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,
		1.0, 1.0, 0.0,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,
		1.0, 0.0, 0.0,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,
		0.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  0.0, 0.0,
		0.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  0.0, 1.0,
		1.0, 1.0, 1.0,  1.0, 1.0, 1.0, 1.0,  1.0, 1.0,
		1.0, 0.0, 1.0,  1.0, 1.0, 1.0, 1.0,  1.0, 0.0,
	};
	static GLuint itx[] = {0,1,3, 1,2,3, 4,5,7, 5,6,7};

	void start_game() {
		::init_engine(800, 600);

		new Res::Texture((char*)"../Media/terrain.png", 0);
		test_obj = GDrawable::create();
		test_obj->vtx_c = sizeof(vtx) / sizeof(GLfloat);
		test_obj->ind_c = sizeof(itx) / sizeof(GLuint);
		test_obj->vertices = vtx;
		test_obj->indices = itx;
		test_obj->tri_mesh_count = sizeof(itx) / sizeof(GLuint) / 3;
		GDrawable::fbind(test_obj);

		projection = glm::perspective(1.0f, float(VMDE->width) / float(VMDE->height), 1.0f, -1.0f);
		view = glm::lookAt(glm::vec3(2.0, 3.0, 2.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

		loop();
	}

	void terminate() {
		GDrawable::dispose(test_obj);
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
