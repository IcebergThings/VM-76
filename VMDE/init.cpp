//=============================================================================
// ■ init.cpp
//-----------------------------------------------------------------------------
//   所有初始化相关的代码都被放置在这里。
//=============================================================================

#include "global.hpp"

//-----------------------------------------------------------------------------
// ● 设置视口
//-----------------------------------------------------------------------------
void setup_viewport() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

//-----------------------------------------------------------------------------
// ● GL数据查询
//-----------------------------------------------------------------------------
void GL_info() {
	log("==========================");
	log("OpenGL Info:");
	GLint a, b;
	glGetIntegerv(GL_MAJOR_VERSION, &a);
	glGetIntegerv(GL_MINOR_VERSION, &b);
	log("Supproted GL Version %d.%d", a, b);
	log("==========================");
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &a);
	log("Maximum vertex attributes: %d", a);
	glGetIntegerv(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &a);
	log("Maximum vertex output components: %d", a);
	glGetIntegerv(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &a);
	log("Maximum fragment shader storage blocks: %d", a);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &a);
	log("Maximum texture size: %d", a);
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &a);
	log("Maximum texture unit: %d", a);
	log("==========================");
}

//-----------------------------------------------------------------------------
// ● 检查GL是否有错误
//-----------------------------------------------------------------------------
void check_gl_error() {
	GLenum error;
	while ((error = glGetError())) switch (error) {
	#define case(x) case x: log(#x); break
	case(GL_INVALID_ENUM);
	case(GL_INVALID_VALUE);
	case(GL_INVALID_OPERATION);
	case(GL_INVALID_FRAMEBUFFER_OPERATION);
	case(GL_OUT_OF_MEMORY);
	default:
		log("unknown GL error: %d", error);
		break;
	#undef case
	}
	error("These are all the errors from glGetError().");
}

//-----------------------------------------------------------------------------
// ● 初始化图形
//-----------------------------------------------------------------------------
void init_graphics(int w, int h, const char* title) {
	init_vmde(w, h);

	// GLFW库初始化
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		log("glfwInit() failed");
		return;
	}

	// OpenGL 向前&向后兼容，使用GL 3.3 Core Profile，窗口大小不可变
	// 如果有更高版本就利用
	// 指定版本后便无需再检查是否支持指定版本，因为GLFW会处理此问题
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(VMDE->width, VMDE->height, title, NULL, NULL);
	if (!window) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		window = glfwCreateWindow(VMDE->width, VMDE->height, title, NULL, NULL);
		VMDE->gl_ver = GL_33;
		if (!window) {
			glfwTerminate();
			error("glfwCreateWindow() (GLFW Window Creation) failed. Your computer need OpenGL 3.3.");
			return;
		}
		log("OpenGL API: 3.3");
	} else {
		VMDE->gl_ver = GL_43;
		log("OpenGL API: 4.3");
	}

	VMSC::init_graphics_state();

	setup_viewport();

	// Query GL info
	GL_info();

	// Setup API constants
	PostProcessingManager::init();
	glDepthRange(0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);

	glDepthFunc(GL_LEQUAL);

	glFrontFace(GL_CCW);
	VMStateControl::enable_depth_test();
	VMStateControl::enable_cullface();
}

//-----------------------------------------------------------------------------
// ● 初始化引擎
//-----------------------------------------------------------------------------
void init_engine(int w, int h, const char* title) {
	Util::init();
	log("initializing the engine");
	srand(time(NULL));
	log("The system is %s-endian.", is_little_endian() ? "little" : "big");

	init_vmde(w, h);
	init_graphics(w, h, title);
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
	VMDE->width = w;
	VMDE->height = h;
	VMDE->done = false;
}
