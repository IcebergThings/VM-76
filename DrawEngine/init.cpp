#include "engine.h"

int init_engine(int w, int h) {
  printf("%s[init_engine]: ENGINE INIT\n", DBG_HEAD);

  if (!glfwInit())
        return -1;

  window = glfwCreateWindow(640, 480, GAME_NAME, NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  return 0;
}
