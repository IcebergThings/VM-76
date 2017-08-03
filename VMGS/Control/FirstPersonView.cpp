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
	void FirstPersonView::init_control() {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);

		keys = {
			.quit = GLFW_KEY_ESCAPE,
			.forward = GLFW_KEY_I,
			.left = GLFW_KEY_J,
			.right = GLFW_KEY_L,
			.back = GLFW_KEY_K,
			.up = GLFW_KEY_SPACE,
			.down = GLFW_KEY_LEFT_SHIFT,
		};
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void FirstPersonView::update_control() {
		// Mouse Input
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		game_player.horizontal_angle -= 0.005 * (xpos - VMDE->width / 2.0);
		game_player.vertical_angle -= 0.005 * (ypos - VMDE->height / 2.0);
		game_player.vertical_angle = glm::clamp(
			- VMath::PIf / 2 + 0.04f,
			game_player.vertical_angle,
			VMath::PIf / 2
		);
		glfwSetCursorPos(window, VMDE->width / 2.0, VMDE->height / 2.0);
		glm::vec3 direction = glm::vec3(
			cos(game_player.vertical_angle) * sin(game_player.horizontal_angle),
			sin(game_player.vertical_angle),
			cos(game_player.vertical_angle) * cos(game_player.horizontal_angle)
		);
		glm::vec3 right = glm::vec3(
			sin(game_player.horizontal_angle - VMath::PIf / 2.0f),
			0,
			cos(game_player.horizontal_angle - VMath::PIf / 2.0f)
		);
		glm::vec3 up = glm::cross(right, direction);
		glm::vec3 cam_pos = game_player.wpos + glm::vec3(0.0, 1.68, 0.0);
		view = glm::lookAt(cam_pos, cam_pos + direction, up);
		view_camera = glm::lookAt(glm::vec3(0.0f), direction, up);

		// Key Input
		glm::vec3 speed = glm::vec3(0.1);
		#define PRESS(k) glfwGetKey(window, keys.k) == GLFW_PRESS
		if (PRESS(quit))
			VMDE->done = true;
		if (PRESS(forward))
			game_player.wpos += glm::vec3(direction.x, 0.0, direction.z) * speed;
		if (PRESS(back))
			game_player.wpos -= glm::vec3(direction.x, 0.0, direction.z) * speed;
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
