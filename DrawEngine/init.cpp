#include "engine.hpp"

void init_shaders() {
}

int init_engine(int w, int h) {
  printf("%s[init_engine]: ENGINE INIT\n", DBG_HEAD);

  // GLFW库初始化
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("glfwInit failed");
  }

  // OpenGL 向前&向后兼容，使用GL 3.2 Core Profile，窗口大小不可变
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  window = glfwCreateWindow(640, 480, GAME_NAME, NULL, NULL);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
  }

  // 设置当前窗口GL上下文
  glfwMakeContextCurrent(window);

  // 初始化GLEW
  if(glewInit() != GLEW_OK) {
    throw std::runtime_error("glewInit failed");
  }

  if(!GLEW_VERSION_3_2) {
    throw std::runtime_error("OpenGL 3.2 API is not available.");
  }

  // 初始化着色器 「OpenGL 3.2没有固定管线了，着色器是被钦定的」
  init_shaders();

  return 0;
}
