//=============================================================================
// ■ GodView.cpp
//-----------------------------------------------------------------------------
//   Game view of God View
//=============================================================================

#include "control.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void GodView::init_control() {
		game = {
			.key_quit = GLFW_KEY_ESCAPE,
			.key_forward = GLFW_KEY_I,
			.key_left = GLFW_KEY_J,
			.key_right = GLFW_KEY_L,
			.key_back = GLFW_KEY_K,
			.key_zoomin = GLFW_KEY_N,
			.key_zoomout = GLFW_KEY_M,
		};
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void GodView::update_control() {
		// Key Input
		const glm::vec3 CameraForward = glm::normalize(glm::vec3(0.0, -1.0, -0.4));

		const glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);

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
		cam.wpos += up * speed;
		if (PRESS(zoomin))
		cam.wpos -= up * speed;
		#undef PRESS

		view = glm::lookAt(cam.wpos - CameraForward * 3.0f, cam.wpos, up);
	}
}
