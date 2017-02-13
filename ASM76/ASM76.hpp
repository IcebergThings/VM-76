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

	enum InstructionSets {
		#define I(x) x,
			#include "instructions.hpp"
		#undef I
	};

	extern uint8_t* global_memory;

	struct Instruct {
		uint16_t opcode;
		uint32_t f;
		uint32_t t;
	};

	void init_environment();

	#define REG(T, P) *((T*) (reg + P))

	class VM {
	private:
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

		void execute();
		void dump_registers();
		void dump_memory();

		static char* decompile(Instruct* prg);
		static Instruct* compile(const char* prg);

		~VM();
	};

}

#endif
