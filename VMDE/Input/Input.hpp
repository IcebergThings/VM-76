//=============================================================================
// ■ Input.hpp
//-----------------------------------------------------------------------------
//   处理输入的模块。
//   统一了键盘、鼠标按键、游戏手柄等输入设备的输入，方便实现鬼畜的键位操作。
//=============================================================================

#pragma once

namespace Input {
	//-------------------------------------------------------------------------
	// ● Unified button input
	//-------------------------------------------------------------------------
	enum ButtonType {
		BUTTON_TYPE_MOUSE,
		BUTTON_TYPE_KEY,
		BUTTON_TYPE_JOYSTICK,
	};
	struct Button {
		ButtonType type;
		int value;
	};
	enum ButtonAction {
		BUTTON_ACTION_DOWN,
		BUTTON_ACTION_UP,
	};
	//-------------------------------------------------------------------------
	// ● Event handlers
	//-------------------------------------------------------------------------
	extern void (*event_button)(Button button, ButtonAction action);
	//-------------------------------------------------------------------------
	// ● Functions
	//-------------------------------------------------------------------------
	void init();
	void update();
	// callbacks for GLFW
	void internal_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
	void internal_mouse_button_callback(GLFWwindow* w, int button, int action, int mods);
}
