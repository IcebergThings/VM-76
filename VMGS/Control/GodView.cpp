// Game view of God View

#include "control.hpp"

#ifdef Control_GodView

#include "GodView.hpp"
namespace VM76 {

	Game_control game = {
		.key_quit = GLFW_KEY_ESCAPE,
		.key_forward = GLFW_KEY_I,
		.key_left = GLFW_KEY_J,
		.key_right = GLFW_KEY_L,
		.key_back = GLFW_KEY_K,
		.key_zoomin = GLFW_KEY_N,
		.key_zoomout = GLFW_KEY_M,
	};

	Camera cam;

	void init_control() { }

	void update_control() {
		// Key Input
		glm::vec3 fwd = glm::normalize(glm::vec3(0.0, 1.0, 0.4));
		glm::vec3 up = glm::normalize(glm::cross(fwd, glm::vec3(1.0, 0.0, 0.0)));
		glm::vec3 speed = glm::vec3(0.1);
		#define PRESS(key) glfwGetKey(window, game.key_##key) == GLFW_PRESS
		if (PRESS(quit)) VMDE->done = true;
		if (PRESS(forward))
		cam.wpos -= glm::vec3(0.0, 0.0, 1.0) * speed;
		if (PRESS(back))
		cam.wpos += glm::vec3(0.0, 0.0, 1.0) * speed;
		if (PRESS(left))
		cam.wpos -= glm::vec3(1.0, 0.0, 0.0) * speed;
		if (PRESS(right))
		cam.wpos += glm::vec3(1.0, 0.0, 0.0) * speed;
		if (PRESS(zoomout))
		cam.wpos += fwd * speed;
		if (PRESS(zoomin))
		cam.wpos -= fwd * speed;
		#undef PRESS

		view = glm::lookAt(cam.wpos + fwd * 3.0f, cam.wpos, up);
	}

}

#endif
