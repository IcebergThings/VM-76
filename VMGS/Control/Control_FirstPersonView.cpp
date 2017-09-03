//=============================================================================
// ■ FirstPersonView.cpp
//-----------------------------------------------------------------------------
//   Game view of First Person Game Control
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void Control_FirstPersonView::init_control(Camera* cam) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(window, .0, .0);
		this->cam = cam;
		keys.quit.type = Input::BUTTON_TYPE_KEY;
		keys.quit.value = GLFW_KEY_ESCAPE;
		keys.forward.type = Input::BUTTON_TYPE_KEY;
		keys.forward.value = GLFW_KEY_W;
		keys.left.type = Input::BUTTON_TYPE_KEY;
		keys.left.value = GLFW_KEY_A;
		keys.right.type = Input::BUTTON_TYPE_KEY;
		keys.right.value = GLFW_KEY_D;
		keys.back.type = Input::BUTTON_TYPE_KEY;
		keys.back.value = GLFW_KEY_S;
		keys.up.type = Input::BUTTON_TYPE_KEY;
		keys.up.value = GLFW_KEY_SPACE;
		keys.down.type = Input::BUTTON_TYPE_KEY;
		keys.down.value = GLFW_KEY_LEFT_SHIFT;
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

		cam->pos = game_player.wpos;
		cam->rotation = glm::vec3(game_player.vertical_angle, game_player.horizontal_angle, 0.0);

		glm::vec3 right = glm::cross(cam->forward, cam->up);

		// Key Input
		glm::vec3 speed = glm::vec3(0.1);
		if (Input::is_pressed(keys.quit))
			VMDE->done = true;
		if (Input::is_pressed(keys.forward))
			game_player.wpos += glm::vec3(cam->forward.x, 0.0, cam->forward.z) * speed;
		if (Input::is_pressed(keys.back))
			game_player.wpos -= glm::vec3(cam->forward.x, 0.0, cam->forward.z) * speed;
		if (Input::is_pressed(keys.left))
			game_player.wpos -= right * speed;
		if (Input::is_pressed(keys.right))
			game_player.wpos += right * speed;
		if (Input::is_pressed(keys.up))
			game_player.wpos += glm::vec3(0.0, 1.0, 0.0) * speed;
		if (Input::is_pressed(keys.down))
			game_player.wpos -= glm::vec3(0.0, 1.0, 0.0) * speed;
	}
}
