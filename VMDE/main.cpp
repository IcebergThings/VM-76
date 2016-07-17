//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "global.hpp"

//-----------------------------------------------------------------------------
// ● 为GLFW提供的通用错误回调函数
//-----------------------------------------------------------------------------
void glfw_error_callback(int error, const char* description) {
	log("GLFW error %d: %s", error, description);
}

//-----------------------------------------------------------------------------
// ● TODO: 这是啥
//-----------------------------------------------------------------------------
glm::mat4 view;

time_t fps_since = time(NULL);
int fps_counter = 0;

//-----------------------------------------------------------------------------
// ● 渲染
//-----------------------------------------------------------------------------
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

		GLint model_location;
		model_location = glGetUniformLocation(main_shader->shaderProgram, "viewMatrix");
		glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(view));
		model_location = glGetUniformLocation(main_shader->shaderProgram, "brightness");
		glUniform1f(model_location, VMDE->state.brightness);

		glUseProgram(main_shader->shaderProgram);

		struct RenderChainNode* chain = render_chain;
		while (chain) {
			VALUE v = chain->n;

			GDrawable::draw(chain->gd);

			chain = chain->next;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//-----------------------------------------------------------------------------
// ● TODO: comment for matrix2D
//-----------------------------------------------------------------------------
void matrix2D() {
	view = glm::ortho(0.0f, float(VMDE->width), 0.0f, float(VMDE->width), -100.0f, 100.0f);
}
