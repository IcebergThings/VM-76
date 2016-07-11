#include "engine.hpp"

int init_engine(int w, int h) {
  printf("%s[init_engine]: ENGINE INIT\n", DBG_HEAD);

  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    throw std::runtime_error("glfwInit failed");

  window = glfwCreateWindow(640, 480, GAME_NAME, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if(glewInit() != GLEW_OK)
    throw std::runtime_error("glewInit failed");

  if(!GLEW_VERSION_3_0)
    throw std::runtime_error("OpenGL 3.0 API is not available.");

  return 0;
}
