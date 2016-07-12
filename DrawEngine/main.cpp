#include "engine.hpp"

void glfw_error_callback(int error, const char* description) {
	printf("%s[glfw_error_callback]: GL Error!\n\t%d: %s\n", DBG_HEAD, error, description);
}

void main_draw_loop() {
	glClear(GL_COLOR_BUFFER_BIT);
	/* Draw whatever you want! */
	glfwSwapBuffers(window);
	glfwPollEvents();
}
