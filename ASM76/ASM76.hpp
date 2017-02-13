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
		uint8_t* local_memory;
		size_t local_memory_size = 0x4000;
		Instruct* instruct_memory;
		static const size_t REGISTER_COUNT = 112;
		uint8_t* reg;

		// Common & special registers
		#define REG(T, P) (*((T*) (reg + P)))
		#define REG100 REG(uint32_t, 100)
		#define REG104 REG(uint32_t, 104)
		#define REG109 REG(uint8_t, 109)
		#define REG110 REG(uint8_t, 110)
		#define REG111 REG(uint8_t, 111)

	public:
		template <class T> T* memfetch(uint32_t address) {
			return address < 0x1000000 ?
				(T*) (global_memory + address) :
				(T*) (local_memory + address - 0x1000000);
		}

		VM(Instruct* program, size_t prg_size);
		~VM();
		void dump_registers();
		void dump_memory();

		void execute();
		void execute_instruction(Instruct*);
		#define I(x) void execute_##x(uint32_t a, uint32_t b);
			#include "instructions.hpp"
		#undef I
	};
}

#endif
