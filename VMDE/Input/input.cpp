//=============================================================================
// ■ input.cpp
//=============================================================================

#include "../VMDE.hpp"

namespace Input {
	//-------------------------------------------------------------------------
	// ● 变量
	//-------------------------------------------------------------------------
	void (*event_button)(Button button, ButtonAction action) = NULL;
	size_t joystick_button_count;
	unsigned char joystick_buttons[INPUT_JOYSTICK_BUTTONS_SIZE];
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		glfwSetKeyCallback(window, internal_key_callback);
		glfwSetMouseButtonCallback(window, internal_mouse_button_callback);
		for (size_t i = 0; i < INPUT_JOYSTICK_BUTTONS_SIZE; i++) {
			joystick_buttons[i] = GLFW_RELEASE;
		}
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//   原则上是每一画格调用此方法一次。
	//-------------------------------------------------------------------------
	void update() {
		glfwPollEvents();
		update_joystick();
	}
	//-------------------------------------------------------------------------
	// ● 更新游戏手柄的输入
	//-------------------------------------------------------------------------
	void update_joystick() {
		int count;
		const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
		joystick_button_count = count;
		// 游戏手柄？不存在的
		if (!buttons || !count) return;
		for (int i = 0; i < count; i++) {
			Button x;
			x.type = BUTTON_TYPE_JOYSTICK;
			x.value = i;

			ButtonAction y;
			if (joystick_buttons[i] == GLFW_RELEASE && buttons[i] == GLFW_PRESS) {
				y = BUTTON_ACTION_DOWN;
			}
			if (joystick_buttons[i] == GLFW_PRESS && buttons[i] == GLFW_RELEASE) {
				y = BUTTON_ACTION_UP;
			}

			// 调用事件处理程序的时候这里的状态表还没更新，不过管它呢
			event_button(x, y);
		}
		memcpy(joystick_buttons, buttons, joystick_button_count);
	}
	//-------------------------------------------------------------------------
	// ● 供GLFW使用的键盘按键回调
	//   不使用scancode——那样的键不应该被这种应用程序使用。
	//-------------------------------------------------------------------------
	void internal_key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
		if (w != window) return;
		if (!event_button) return;

		Button x;
		x.type = BUTTON_TYPE_KEY;
		x.value = key;

		ButtonAction y;
		switch (action) {
		case GLFW_PRESS:
			y = BUTTON_ACTION_DOWN;
			break;
		case GLFW_RELEASE:
			y = BUTTON_ACTION_UP;
			break;
		case GLFW_REPEAT:
		default:
			return;
		}

		event_button(x, y);
	}
	//-------------------------------------------------------------------------
	// ● 供GLFW使用的鼠标键回调
	//-------------------------------------------------------------------------
	void internal_mouse_button_callback(GLFWwindow* w, int button, int action, int mods) {
		if (w != window) return;
		if (!event_button) return;

		Button x;
		x.type = BUTTON_TYPE_MOUSE;
		x.value = button;

		ButtonAction y;
		switch (action) {
		case GLFW_PRESS:
			y = BUTTON_ACTION_DOWN;
			break;
		case GLFW_RELEASE:
			y = BUTTON_ACTION_UP;
			break;
		}

		event_button(x, y);
	}
}
