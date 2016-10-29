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

	#define RCN struct RenderChainNode
	struct RenderChainNode {
		RenderChainNode* prev;
		GDrawable::GDrawable* gd;
		bool visible;
		RenderChainNode* next;
	};

	// VMDE操控的全局变量
	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern RenderChainNode* render_chain;
#endif
