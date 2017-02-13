//=============================================================================
// ■ ASM76.hpp
//=============================================================================

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdint>

#ifndef _INCLUDE_ASM76_H
#define _INCLUDE_ASM76_H

namespace ASM76 {
	//-------------------------------------------------------------------------
	// ● 指令结构的定义
	//-------------------------------------------------------------------------
	enum InstructionOpcode {
		#define I(x) x,
			#include "instructions.hpp"
		#undef I
	};
	struct Instruct {
		uint16_t opcode;
		uint32_t a;
		uint32_t b;
	};
	//-------------------------------------------------------------------------
	// ● 全局变量
	//-------------------------------------------------------------------------
	extern uint8_t global_memory[];
	//-------------------------------------------------------------------------
	// ● 实用函数
	//-------------------------------------------------------------------------
	void init();
	Instruct* compile(const char* prg);
	char* decompile(Instruct* prg);
	//-------------------------------------------------------------------------
	// ● Virtual Machine类
	//-------------------------------------------------------------------------
	class VM {
	private:
	#define REG(T, P) *((T*) (reg + P))
		uint8_t* local_memory;
		size_t local_mem_size = 0x4000;
		Instruct* instruct_memory;
		uint8_t* reg;

		// Common & Special register
		uint32_t* REG86;
		uint32_t* REG90;
		uint8_t* REG97;
		uint8_t* REG98;
		uint8_t* REG99;

	public:
		template <class T> T* memfetch(uint32_t address) {
			return address < 0x1000000 ?
				(T*) (global_memory + address) :
				(T*) (local_memory + address - 0x1000000);
		}

		VM(Instruct* program, size_t prg_size);
		~VM();

		void execute();
		void execute_instruction(Instruct*);
		#define I(x) void execute_##x(uint32_t a, uint32_t b);
			#include "instructions.hpp"
		#undef I
		void dump_registers();
		void dump_memory();
	};
}

#endif
