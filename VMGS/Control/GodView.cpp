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
	void GodView::init_control(Camera* cam) {
		this->cam = cam;

		keys = {
			.quit = GLFW_KEY_ESCAPE,
			.forward = GLFW_KEY_I,
			.left = GLFW_KEY_J,
			.right = GLFW_KEY_L,
			.back = GLFW_KEY_K,
			.zoomin = GLFW_KEY_N,
			.zoomout = GLFW_KEY_M,
		};
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void GodView::update_control() {
		cam->rotate = glm::vec3(1.0, 0.0, 0.0);

		// Key Input
		glm::vec3 speed = glm::vec3(0.1);
		#define PRESS(k) glfwGetKey(window, keys.k) == GLFW_PRESS
		if (PRESS(quit))
			VMDE->done = true;
		if (PRESS(forward))
			cam->wpos += glm::vec3(0.0, 0.0, 1.0) * speed;
		if (PRESS(back))
			cam->wpos -= glm::vec3(0.0, 0.0, 1.0) * speed;
		if (PRESS(left))
			cam->wpos += glm::vec3(1.0, 0.0, 0.0) * speed;
		if (PRESS(right))
			cam->wpos -= glm::vec3(1.0, 0.0, 0.0) * speed;
		if (PRESS(zoomout))
			cam->wpos += glm::vec3(0.0, 1.0, 0.0) * speed;
		if (PRESS(zoomin))
			cam->wpos -= glm::vec3(0.0, 1.0, 0.0) * speed;
		#undef PRESS
	}
}
