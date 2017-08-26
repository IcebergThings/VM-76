//=============================================================================
// ■ VMGS/Scene/scene.cpp
//-----------------------------------------------------------------------------
//   这个类是应该被称为Scene_Base的吗？
//=============================================================================

#include "../VMGS.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 更新
	//-------------------------------------------------------------------------
	void Scene::update() {
	}
	//-------------------------------------------------------------------------
	// ● 绘制
	//-------------------------------------------------------------------------
	void Scene::render() {
	}
	//-------------------------------------------------------------------------
	// ● 按键
	//-------------------------------------------------------------------------
	void Scene::event_button(Input::Button button, Input::ButtonAction action) {
		if (button.type == Input::BUTTON_TYPE_KEY
			&& button.value == GLFW_KEY_PAUSE
			&& action == Input::BUTTON_ACTION_DOWN) {
			abort();
		} else if (button.type == Input::BUTTON_TYPE_KEY
			&& button.value == GLFW_KEY_F3
			&& action == Input::BUTTON_ACTION_DOWN) {
			SceneManager::debug_info_visible = !SceneManager::debug_info_visible;
		}
	}
}
