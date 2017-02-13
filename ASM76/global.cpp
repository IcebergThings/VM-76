//=============================================================================
// ■ ASM76/global.cpp
//-----------------------------------------------------------------------------
//   全局变量与实用函数。
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 {
	//-------------------------------------------------------------------------
	// ● 全局变量
	//-------------------------------------------------------------------------
	// 4MB program useable
	// 12MB IO interface
	uint8_t global_memory[0x1000000] = {0};
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		printf("Init ASM76 environment\n");
		// nothing to do here now
	}
	//-------------------------------------------------------------------------
	// ● 汇编
	//-------------------------------------------------------------------------
	Instruct* compile(const char* prg) {
		return NULL;
	}
	//-------------------------------------------------------------------------
	// ● 反汇编
	//-------------------------------------------------------------------------
	char* decompile(Instruct* prg) {
		return NULL;
	}
}