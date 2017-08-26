//=============================================================================
// ■ input.cpp
//-----------------------------------------------------------------------------
//   TODO
//=============================================================================

#include "../VMDE.hpp"

namespace Input {
	//-------------------------------------------------------------------------
	// ● Event handlers
	//-------------------------------------------------------------------------
	void (*event_button)(Button button, ButtonAction action) = NULL;
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		glfwSetKeyCallback(window, internal_key_callback);
		glfwSetMouseButtonCallback(window, internal_mouse_button_callback);
	}
	//-------------------------------------------------------------------------
	// ● 更新
	//   原则上是每一画格调用此方法一次。
	//-------------------------------------------------------------------------
	void update() {
		glfwPollEvents();
		// TODO: add joystick support
	}
	//-------------------------------------------------------------------------
	// ● 供GLFW使用的按键回调
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
