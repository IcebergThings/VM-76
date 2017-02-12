//==============================================================================
// ■ ASM76.h
//==============================================================================

#include "stdlib.h"
#include "string.h"
#include "cstdio"
#include "stdint.h"

#ifndef _INCLUDE_ASM76_H
#define _INCLUDE_ASM76_H

namespace ASM76 {

	enum InstructionSets {
		NOOP, // No Operation is always NULL

		// 76-Base
		LCMM,
		LDLA, LDIA, LDBA,
		LDLR, LDIR, LDBR,
		SLLA, SLIA, SLBA,
		SLLR, SLIR, SLBR,
		DATL, DATI, DATB,
		ADDL, ADDI, ADDB,
		MINL, MINI, MINB,
		MTPL, MTPI, MTPB,
		DIVL, DIVI, DIVB,
		MODL, MODI, MODB,
		_HLT,

		// Logistics & Flow control
		ANDL, ANDI, ANDB,
		OR_L, OR_I, OR_B,
		NOTL, NOTI, NOTB,
		XORL, XORI, XORB,
		CMPL, CMPI, CMPB,
		JMPR,
		JMPA,
		JI9R, JI8R, JI7R,
		CALR, CALA,
		PUSH,
		POP,

		// 76 Float

		// 76 Vectors

		// BIOS Instructions
	};

	extern uint8_t* global_memory;

	struct Instruct {
		uint16_t opcode;
		uint32_t f;
		uint32_t t;
	};

	void init_environment();

	#define REG(T, P) *((T*) reg + P)

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
