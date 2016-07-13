//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

#include "global.hpp"

char temp_src[] = "#version 300 core\nvoid main() {}";

int init_engine(int w, int h) {
	log("initializing the engine");

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
		rb_raise(rb_eRuntimeError, "glfwCreateWindow() (GLFW Window Creation) failed. Your computer need OpenGL 3.2.");
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
	if (glewInit() != GLEW_OK) rb_raise(rb_eRuntimeError, "glewInit() (GLEW Initialization) failed.");

  setup_viewport();

  glGenBuffers(1, &VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 初始化着色器「OpenGL 3.2没有固定管线了，着色器是被钦定的」
	init_shaders();

	return 0;
}

void init_shaders() {
  GLchar* basic_2D_vsh_src = (GLchar*) &temp_src;

  basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(basic_2D_vsh, 1, &basic_2D_vsh_src, NULL);
  glCompileShader(basic_2D_vsh);
}

void setup_viewport() {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
}
