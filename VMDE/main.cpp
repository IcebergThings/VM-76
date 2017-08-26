//=============================================================================
// ■ main.cpp
//-----------------------------------------------------------------------------
//   处理主渲染循环等主要过程。
//=============================================================================

#include "VMDE.hpp"

//-----------------------------------------------------------------------------
// ● 为GLFW提供的通用错误回调函数
//-----------------------------------------------------------------------------
void glfw_error_callback(int error, const char* description) {
	log("GLFW error %d: %s", error, description);
}

//-----------------------------------------------------------------------------
// ● FPS计数
//-----------------------------------------------------------------------------
#include <chrono>

auto fps_since = chrono::high_resolution_clock::now();
int fps_counter = 0;
double accumulated_frame_time = 0;

//-----------------------------------------------------------------------------
// ● 渲染
//-----------------------------------------------------------------------------
auto now = chrono::high_resolution_clock::now();

void main_draw_start() {
	Input::update();
	if (glfwWindowShouldClose(window)) VMDE->done = true;
	VMDE->frame_count++;
	fps_counter++;

	now = chrono::high_resolution_clock::now();
	chrono::duration<double, micro> delay = now - fps_since;
	fps_since = now;

	VMDE->fps = glm::round(1000000.0 / ((double) delay.count()));
	VMDE->frame_time = ((double) delay.count()) / 1000.0;
}

void main_draw_end() {
	if (!VMDE->state.frozen) {
		glFlush();
		glfwSwapBuffers(window);
	}
}
