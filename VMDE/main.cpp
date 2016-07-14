//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "global.hpp"

void glfw_error_callback(int error, const char* description) {
	log("GLFW error %d: %s", error, description);
}

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

		RenderChainNode* chain = render_chain;
		while (chain != NULL) {
			VALUE v = chain->n;
			chain = chain->next;
		}

		glUseProgram(main_shader->shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
