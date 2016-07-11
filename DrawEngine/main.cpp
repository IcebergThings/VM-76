#include "engine.hpp"

void glfw_error_callback() {

}

void main_draw_loop() {
  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  glLoadIdentity();

  glBegin(GL_TRIANGLES);

    glColor3f(1.0, 0.0, 0.0);    // Red
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);    // Green
    glVertex3f(-1.0, -1.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);    // Blue
    glVertex3f(1.0, -1.0, 0.0);

  glEnd();

  /* Swap front and back buffers */
  glfwSwapBuffers(window);

  /* Poll for and process events */
  glfwPollEvents();
}
