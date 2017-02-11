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
		printf("Init ASM76 env\n");
		global_memory = new uint8_t[0x1000000];
		memset(global_memory, 0, 0x1000000);
	}

	VM::VM(Instruct* program, size_t prg_size) {
		// 16K local memory in default
		local_memory = new uint8_t[local_mem_size];
		memset(local_memory, 0, local_mem_size);
		instruct_memory = (Instruct*) local_memory;
		printf("init memory with program sized %zu\n", prg_size);
		memcpy(instruct_memory, program, prg_size);

		// 99 registers
		reg = new uint8_t[100];
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

	#define OPC(code) (now->opcode == code)

	void VM::execute() {
		Instruct* now = memfetch<Instruct>(*REG86);
		while (!OPC(_HLT)) {
			printf("%x : %x, %x, %x\n", *REG86, now->opcode, now->f, now->t);

			if OPC(LCMM) {
				uint8_t* new_mem = new uint8_t[now->f];
				memset(new_mem, 0, now->f);
				memcpy(new_mem, local_memory, local_mem_size);
				free(local_memory);
				local_mem_size = now->f;
				local_memory = new_mem;
				instruct_memory = (Instruct*) new_mem;

				printf("Changed local size to %zu bytes\n", local_mem_size);
			} else if OPC(LDLA) {
				REG(uint64_t, now->t) = *memfetch<uint64_t>(now->f);
			} else if OPC(LDIA) {
				REG(uint32_t, now->t) = *memfetch<uint32_t>(now->f);
			} else if OPC(LDBA) {
				REG(uint8_t, now->t) = *memfetch<uint8_t>(now->f);
			} else if OPC(LDLR) {
				REG(uint64_t, now->t) = *memfetch<uint64_t>(REG(uint32_t, now->f));
			} else if OPC(LDIR) {
				REG(uint32_t, now->t) = *memfetch<uint32_t>(REG(uint32_t, now->f));
			} else if OPC(LDBR) {
				REG(uint8_t, now->t) = *memfetch<uint8_t>(REG(uint32_t, now->f));
			} else if OPC(SLLA) {
				*memfetch<uint64_t>(now->f) = REG(uint64_t, now->t);
			} else if OPC(SLIA) {
				*memfetch<uint32_t>(now->f) = REG(uint32_t, now->t);
			} else if OPC(SLBA) {
				*memfetch<uint8_t>(now->f) = REG(uint8_t, now->t);
			} else if OPC(SLLR) {
				*memfetch<uint64_t>(REG(uint32_t, now->f)) = REG(uint64_t, now->t);
			} else if OPC(SLIR) {
				*memfetch<uint32_t>(REG(uint32_t, now->f)) = REG(uint32_t, now->t);
			} else if OPC(SLBR) {
				*memfetch<uint8_t>(REG(uint32_t, now->f)) = REG(uint8_t, now->t);
			}

			*REG86 += sizeof(Instruct);
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
