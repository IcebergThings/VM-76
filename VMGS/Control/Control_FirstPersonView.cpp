//=============================================================================
// ■ FirstPersonView.cpp
//-----------------------------------------------------------------------------
//   Game view of First Person Game Control
//=============================================================================

#include "control.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void Control_FirstPersonView::init_control(Camera* cam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, .0, .0);
		this->cam = cam;
		keys = {
			.quit = GLFW_KEY_ESCAPE,
			.forward = GLFW_KEY_W,
			.left = GLFW_KEY_A,
			.right = GLFW_KEY_D,
			.back = GLFW_KEY_S,
			.up = GLFW_KEY_SPACE,
			.down = GLFW_KEY_LEFT_SHIFT,
		};
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void Control_FirstPersonView::update_control() {
		// Mouse Input
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, .0, .0);
		game_player.horizontal_angle -= 0.005 * xpos;
		game_player.vertical_angle = glm::clamp(
			-PI / 2 + 0.04,
			game_player.vertical_angle + 0.005 * ypos,
			PI / 2
		);

		cam->wpos = game_player.wpos;
		cam->rotate = glm::vec3(game_player.vertical_angle, game_player.horizontal_angle, 0.0);

		glm::vec3 right = glm::cross(cam->forward, cam->up);

		// Key Input
		glm::vec3 speed = glm::vec3(0.1);
		#define PRESS(k) glfwGetKey(window, keys.k) == GLFW_PRESS
		if (PRESS(quit))
			VMDE->done = true;
		if (PRESS(forward))
			game_player.wpos += glm::vec3(cam->forward.x, 0.0, cam->forward.z) * speed;
		if (PRESS(back))
			game_player.wpos -= glm::vec3(cam->forward.x, 0.0, cam->forward.z) * speed;
		if (PRESS(left))
			game_player.wpos -= right * speed;
		if (PRESS(right))
			game_player.wpos += right * speed;
		if (PRESS(up))
			game_player.wpos += glm::vec3(0.0, 1.0, 0.0) * speed;
		if (PRESS(down))
			game_player.wpos -= glm::vec3(0.0, 1.0, 0.0) * speed;
		#undef PRESS
	}

}
