#include "engine.h"

void main_draw_loop() {
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Swap front and back buffers */
  glfwSwapBuffers(window);

  /* Poll for and process events */
  glfwPollEvents();
}
