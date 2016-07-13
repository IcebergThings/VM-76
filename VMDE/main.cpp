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
		glClear(GL_COLOR_BUFFER_BIT);
		/* Render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
