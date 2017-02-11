//==============================================================================
// ASM76/VM.cpp
//==============================================================================
// Virtual Machine
//==============================================================================

#include "ASM76.hpp"

namespace ASM76 {

	uint8_t* global_memory;

	void init_environment() {
		// 4MB program useable
		// 12MB IO interface
		global_memory = new uint8_t[0x1000000];
		memset(global_memory, 0, 0x1000000);
	}

	VM::VM(Instruct* program, size_t prg_size) {
		// 16K local memory in default
		local_memory = new uint8_t[0x4000];
		memset(local_memory, 0, 0x4000);
		memcpy(instruct_memory, program, prg_size);

		// 99 registers
		reg = new uint8_t[99];
		memset(reg, 0, 99);

		// Setup registers
		// Instruction pointer
		REG(uint32_t, 86) = 0x1000000;
		// Stack bottom pointer
		REG(uint32_t, 90) = 0x1003000;

		REG86 = (uint32_t*) reg + 86;
		REG90 = (uint32_t*) reg + 90;
		REG97 = (uint8_t*) reg + 97;
		REG98 = (uint8_t*) reg + 98;
		REG99 = (uint8_t*) reg + 99;
	}

	void VM::execute() {
		Instruct* now = memfetch<Instruct>(*REG86);
		while (now->opcode != _HLT) {
			printf("%x : %x, %x, %x", *REG86, now->opcode, now->f, now->t);

			*REG86 ++;
			now = memfetch<Instruct>(*REG86);
		}
	}

	void VM::dump_registers() {}
	void VM::dump_memory() {}

	char* VM::decompile(Instruct* prg) {
		return NULL;
	}

	Instruct* VM::compile(const char* prg) {
		return NULL;
	}

	VM::~VM() {
		free(local_memory);
		free(reg);
	}

}
