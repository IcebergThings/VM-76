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
glm::mat4 projection, view = glm::mat4(1.0);

time_t fps_since = time(NULL);
int fps_counter = 0;
time_t accumulated_frame_time = 0;

//-----------------------------------------------------------------------------
// ● 渲染
//-----------------------------------------------------------------------------
time_t now;

void main_draw_start() {
	glfwPollEvents();
	if (glfwWindowShouldClose(window)) VMDE->done = true;
	VMDE->frame_count++;
	fps_counter++;
	now = time(NULL);
	if (difftime(now, fps_since) > 0)
		VMDE->fps = fps_counter / difftime(now, fps_since);
	if (difftime(now, fps_since) >= 1.0) {
		fps_counter = 0;
		fps_since = now;
		VMDE->frame_time = accumulated_frame_time / (double) VMDE->fps * 1000.0;
		accumulated_frame_time -= 1.0;
	}
}

void main_draw_end() {
	if (!VMDE->state.frozen) {
		glFlush();

		accumulated_frame_time += difftime(time(NULL), now);

		glfwSwapBuffers(window);
	}
}
