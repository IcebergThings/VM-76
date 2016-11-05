//=============================================================================
// ■ VMDE.hpp
//-----------------------------------------------------------------------------
//   VMDE头文件。
//=============================================================================

#ifndef _INCLUDE_VMDE_H
	#define _INCLUDE_VMDE_H
	// 全局事件
	EXPORTED bool (*on_terminate)();
	EXPORTED void (*on_key)(int key, int scancode, int action, int mode);
#endif
