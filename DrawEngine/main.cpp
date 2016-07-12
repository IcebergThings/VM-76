#include "engine.hpp"

void glfw_error_callback(int error, const char* description) {
  printf("%s[glfw_error_callback]: GL Error!\n  %d: %s\n", DBG_HEAD, error, description);
}

void main_draw_loop() {

  if (!VMDE->States->freeze) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

}
