//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "global.hpp"

void glfw_error_callback(int error, const char* description) {
	printf(DBG_HEAD "[glfw_error_callback]: GL Error!\n\t%d: %s\n", error, description);
}

// Why the fucking long **double**? Is it ever portable?
long double system_time = time(NULL);
long double system_time_millisecond = system_time * 1000;
int temp_fps_c = 0;

void main_draw_loop() {
	VMDE->frame_count++;
	temp_fps_c++;
	if (time(NULL) >= system_time + 1) {
		VMDE->fps = temp_fps_c;
		temp_fps_c = 0;

		system_time = time(NULL);
		system_time_millisecond = system_time * 1000;
	}
	if (!VMDE->States->freeze) {
		glClear(GL_COLOR_BUFFER_BIT);
		/* Render */
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
