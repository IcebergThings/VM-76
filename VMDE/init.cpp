//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

#include "global.hpp"

GLuint basic_2D_vsh;
GLuint basic_2D_fsh;

void setup_viewport() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void init_engine(int w, int h) {
	log("initializing the engine");

	// 初始化VMDE结构
	VMDE = (struct VMDE*) malloc(sizeof(struct VMDE));
	VMDE->state.frozen = false;
	VMDE->state.brightness = 1.0;

	VMDE->frame_count = 0;
	VMDE->fps = 0;
	VMDE->width = w; VMDE->height = h;

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
		rb_raise(rb_eRuntimeError, "glfwCreateWindow() (GLFW Window Creation) failed. Your computer need OpenGL 3.2.");
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) rb_raise(rb_eRuntimeError, "glewInit() (GLEW Initialization) failed.");

	setup_viewport();

	// 初始化着色器「OpenGL 3.2没有固定管线了，着色器是被钦定的」
	main_shader = new Shaders();
	main_shader->init_shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
	main_shader->link_program();
}
