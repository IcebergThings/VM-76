//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

#include "global.hpp"

int init_engine(int w, int h) {
	printf(DBG_HEAD "[init_engine]: initializing the engine\n");

	// 初始化VMDE结构
	VMDE = (VMDE_t*) malloc(sizeof(VMDE));
	VMDE->States = (VMDE_State_t*) malloc(sizeof(VMDE->States));
	VMDE->States->freeze = false;
	VMDE->frame_count = 0;
	VMDE->fps = 0;

	// GLFW库初始化
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) rb_raise(rb_eRuntimeError, "glfwInit() failed");

	// OpenGL 向前&向后兼容，使用GL 3.2 Core Profile，窗口大小不可变
	// 指定版本后便无需再检查是否支持指定版本，因为GLFW会处理此问题
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(w, h, GAME_NAME, NULL, NULL);
	if (!window) {
		glfwTerminate();
		rb_raise(rb_eRuntimeError, "glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
	if (glewInit() != GLEW_OK) rb_raise(rb_eRuntimeError, "glewInit() failed");

	// 初始化着色器「OpenGL 3.2没有固定管线了，着色器是被钦定的」
	init_shaders();

	return 0;
}

void init_shaders() {
	// TODO
}
