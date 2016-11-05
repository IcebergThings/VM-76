//=============================================================================
// ■ VMDE.hpp
//-----------------------------------------------------------------------------
//   VMDE头文件。
//=============================================================================

#ifndef _INCLUDE_VMDE_H
	#define _INCLUDE_VMDE_H
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif
	// 全局事件
	EXPORTED bool (*on_terminate)();
	EXPORTED void (*on_key)(int key, int scancode, int action, int mode);
	#undef EXPORTED
#endif
