//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "global.hpp"

void glfw_error_callback(int error, const char* description) {
	log("GLFW error %d: %s", error, description);
}

glm::mat4 view;

time_t fps_since = time(NULL);
int fps_counter = 0;

void main_draw_loop() {
	VMDE->frame_count++;
	fps_counter++;
	time_t now = time(NULL);
	if (difftime(now, fps_since) >= 1.0d) {
		VMDE->fps = fps_counter;
		fps_counter = 0;
		fps_since = now;
	}
	if (!VMDE->state.frozen) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		/* Render */

		GLint modelLoc = glGetUniformLocation(main_shader->shaderProgram, "viewMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(view));
		modelLoc = glGetUniformLocation(main_shader->shaderProgram, "brightness");
		glUniform1f(modelLoc, VMDE->state.brightness);

		glUseProgram(main_shader->shaderProgram);

		struct RenderChainNode* chain = render_chain;
		while (chain != NULL) {
			VALUE v = chain->n;

			GDrawable::draw(chain->gd);

			chain = chain->next;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void main_set_brightness(float b) {
	VMDE->state.brightness = b;
}

void matrix2D() {
	view = glm::ortho(0.0f, float(VMDE->width), 0.0f, float(VMDE->width), -100.0f, 100.0f);
}
