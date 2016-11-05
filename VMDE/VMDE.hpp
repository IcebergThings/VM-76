//=============================================================================
// ■ VMDE.hpp
//-----------------------------------------------------------------------------
//   VMDE头文件。
//=============================================================================

#include "GDrawable.hpp"

#ifndef _INCLUDE_VMDE_H
	#define _INCLUDE_VMDE_H

	struct VMDEState {
		bool frozen;
		float brightness;
	};

	struct VMDE {
		VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
		double frame_time;
	};

	// VMDE操控的全局变量
	extern struct VMDE* VMDE;
	extern GLFWwindow* window;

	// 关闭函数要在client里
	extern "C" {
		extern void client_terminate();
		extern void i_have_a_key(GLFWwindow* window, int key, int scancode, int action, int mode);
	}

#endif
