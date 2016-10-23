//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

#include "VMDE.hpp"
#include "audio.hpp"

GLuint basic_2D_vsh;
GLuint basic_2D_fsh;

//-----------------------------------------------------------------------------
// ● 设置视口
//-----------------------------------------------------------------------------
void setup_viewport() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// ● 初始化图形
//-----------------------------------------------------------------------------
void init_graphics(int w, int h) {
	init_vmde(w, h);

	// GLFW库初始化
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		log("glfwInit() failed");
		return;
	}

	// OpenGL 向前&向后兼容，使用GL 3.3 Core Profile，窗口大小不可变
	// 指定版本后便无需再检查是否支持指定版本，因为GLFW会处理此问题
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(VMDE->width, VMDE->height, GAME_NAME, NULL, NULL);
	if (!window) {
		glfwTerminate();
		log("glfwCreateWindow() (GLFW Window Creation) failed. Your computer need OpenGL 3.2.");
		return;
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);
	// 垂直同步，拒绝鬼畜
	glfwSwapInterval(1);
	// 深度测试是必要的
	glEnable(GL_DEPTH_TEST);
	// 混合是极度必要的
	glEnable(GL_BLEND);

	// 初始化GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		log("glewInit() (GLEW Initialization) failed.");
		return;
	}

	setup_viewport();

	reload_shaders();

	// 获取可用的Vertex Attributes数量
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	log("Maximum nr of vertex attributes supported: %d\n", nrAttributes);
}

void reload_shaders() {
	// 初始化着色器「OpenGL 3.1开始没有固定管线了，着色器是被钦定的」
	xefree(main_shader);
	main_shader = new Shaders();
	main_shader->init_shaders(temp_vertexShaderSource, temp_fragmentShaderSource);
	main_shader->link_program();
}

//-----------------------------------------------------------------------------
// ● 初始化引擎
//-----------------------------------------------------------------------------
void init_engine(int w, int h) {
	log("initializing the engine");

	srand(time(NULL));

	init_vmde(w, h);
	init_graphics(w, h);

	// 初始化声音
	Audio::init();

	log("initialized the engine");
}

//-----------------------------------------------------------------------------
// ● 初始化VMDE结构体
//-----------------------------------------------------------------------------
void init_vmde(int w, int h) {
	VMDE = new struct VMDE;
	VMDE->state.frozen = false;
	VMDE->state.brightness = 1.0;

	VMDE->frame_count = 0;
	VMDE->fps = 0;
	VMDE->width = w; VMDE->height = h;
}
