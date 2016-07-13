#include "global.hpp"

void init_RClass() {
  GResPic = rb_define_class_under(Global_module, "GRes_Picture", rb_cObject);

  rb_define_method(GResPic, "load_pic", (RB_F_R) warp_load_pic, 1);

}

void init_RModule() {
  Global_module = rb_define_module("VMDE");

  rb_define_module_function(Global_module, "init_engine", (RB_F_R) wrap_init_engine, 2);
  rb_define_module_function(Global_module, "update", (RB_F_R) warp_main_draw_loop, 0);
  rb_define_module_function(Global_module, "get_frame_count", (RB_F_R) warp_main_get_frame_count, 0);
  rb_define_module_function(Global_module, "get_fps", (RB_F_R) warp_main_get_fps, 0);

}

char temp_src[] = "#version 300 core\n void main() {}";

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

int init_engine(int w, int h) {
  printf("%s[init_engine]: ENGINE INIT\n", DBG_HEAD);

  // 初始化VMDE结构
  VMDE = (VMDE_t*) malloc(sizeof(VMDE));
  VMDE->States = (VMDE_State_t*) malloc(sizeof(VMDE->States));
  VMDE->States->freeze = false;
  VMDE->framecount = 0;
  VMDE->fps = 0;


	// GLFW库初始化
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		printf("%s[init_engine]: GLFW Init Failed\n", DBG_HEAD);
    exit(1);
	}

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
    printf("%s[init_engine]: GLFW Window Creation Failed. Your computer need OpenGL 3.2\n", DBG_HEAD);
    exit(1);
	}

	// 设置当前窗口GL上下文
	glfwMakeContextCurrent(window);

	// 初始化GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("%s[init_engine]: GLEW Inition failed. Your computer need OpenGL 3.2\n", DBG_HEAD);
    exit(1);
	}

  setup_viewport();

  glGenBuffers(1, &VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

	// 初始化着色器「OpenGL 3.2没有固定管线了，着色器是被钦定的」
	init_shaders();

	return 0;
}
