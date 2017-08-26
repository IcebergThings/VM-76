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
	// ● States
	//-------------------------------------------------------------------------
	// 随便定了个最大值，我也不知道有没有100键的手柄
	// 不过要是谁做了个键盘当手柄的驱动那就另当别论了
	#define INPUT_JOYSTICK_BUTTONS_SIZE ((size_t) 100)
	extern size_t joystick_button_count;
	extern unsigned char joystick_buttons[INPUT_JOYSTICK_BUTTONS_SIZE];
	//-------------------------------------------------------------------------
	// ● Functions
	//-------------------------------------------------------------------------
	void init();
	void update();
	void update_joystick();
	bool is_pressed(Button button);
	// callbacks for GLFW
	void internal_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods);
	void internal_mouse_button_callback(GLFWwindow* w, int button, int action, int mods);
}
