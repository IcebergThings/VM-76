//=============================================================================
// ■ ASM76/VM.cpp
//-----------------------------------------------------------------------------
//   Virtual Machine
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 {
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
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

		REG86 = (uint32_t*) (reg + 86);
		REG90 = (uint32_t*) (reg + 90);
		REG97 = (uint8_t*) (reg + 97);
		REG98 = (uint8_t*) (reg + 98);
		REG99 = (uint8_t*) (reg + 99);
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	VM::~VM() {
		free(local_memory);
		free(reg);
	}
	//-------------------------------------------------------------------------
	// ● 解释
	//-------------------------------------------------------------------------
	void VM::execute() {
		#define OPC(code) (now->opcode == code)
		Instruct* now = memfetch<Instruct>(*REG86);
		while (!OPC(_HLT)) {
			printf("%08x: %04x, %x, %x\n", *REG86, now->opcode, now->a, now->b);
			VM::execute_instruction(now);
			*REG86 += sizeof(Instruct);
			now = memfetch<Instruct>(*REG86);
		}
	}
	//-------------------------------------------------------------------------
	// ● 解释一条指令
	//-------------------------------------------------------------------------
	void VM::execute_instruction(Instruct* i) {
		uint32_t a = i->a;
		uint32_t b = i->b;
		switch (i->opcode) {
		// ===========================
		//  76-Base
		// ===========================
		// LCMM
		case LCMM: {
			uint8_t* new_mem = new uint8_t[a];
			memset(new_mem, 0, a);
			size_t copied_size = a < local_mem_size ? a : local_mem_size;
			memcpy(new_mem, local_memory, copied_size);
			free(local_memory);
			local_mem_size = a;
			local_memory = new_mem;
			instruct_memory = (Instruct*) new_mem;
			printf("Changed local size to %zu bytes\n", local_mem_size);
			break;
		}
		// Load Data
		case LDLA:
			REG(uint64_t, b) = *memfetch<uint64_t>(a);
			break;
		case LDIA:
			REG(uint32_t, b) = *memfetch<uint32_t>(a);
			break;
		case LDBA:
			REG(uint8_t, b) = *memfetch<uint8_t>(a);
			break;
		case LDLR:
			REG(uint64_t, b) = *memfetch<uint64_t>(REG(uint32_t, a));
			break;
		case LDIR:
			REG(uint32_t, b) = *memfetch<uint32_t>(REG(uint32_t, a));
			break;
		case LDBR:
			REG(uint8_t, b) = *memfetch<uint8_t>(REG(uint32_t, a));
			break;
		// Store data
		case SLLA:
			*memfetch<uint64_t>(a) = REG(uint64_t, b);
			break;
		case SLIA:
			*memfetch<uint32_t>(a) = REG(uint32_t, b);
			break;
		case SLBA:
			*memfetch<uint8_t>(a) = REG(uint8_t, b);
			break;
		case SLLR:
			*memfetch<uint64_t>(REG(uint32_t, a)) = REG(uint64_t, b);
			break;
		case SLIR:
			*memfetch<uint32_t>(REG(uint32_t, a)) = REG(uint32_t, b);
			break;
		case SLBR:
			*memfetch<uint8_t>(REG(uint32_t, a)) = REG(uint8_t, b);
			break;
		case DATL:
			REG(uint64_t, b) = (uint64_t) a;
			break;
		case DATI:
			REG(uint32_t, b) = (uint32_t) a;
			break;
		case DATB:
			REG(uint8_t, b) = (uint8_t) a;
			break;
		// Mem operation
		case MOVL:
			*memfetch<uint64_t>(b) = *memfetch<uint64_t>(a);
			break;
		case MOVI:
			*memfetch<uint32_t>(b) = *memfetch<uint32_t>(a);
			break;
		case MOVB:
			*memfetch<uint8_t>(b) = *memfetch<uint8_t>(a);
			break;
		case MVPL:
			*memfetch<uint64_t>(REG(uint32_t, b)) = *memfetch<uint64_t>(REG(uint32_t, a));
			break;
		case MVPI:
			*memfetch<uint32_t>(REG(uint32_t, b)) = *memfetch<uint32_t>(REG(uint32_t, a));
			break;
		case MVPB:
			*memfetch<uint8_t>(REG(uint32_t, b)) = *memfetch<uint8_t>(REG(uint32_t, a));
			break;
		case MVRL:
			REG(uint64_t, b) = REG(uint64_t, a);
			break;
		case MVRI:
			REG(uint32_t, b) = REG(uint32_t, a);
			break;
		case MVRB:
			REG(uint8_t, b) = REG(uint8_t, a);
			break;
		// Basic Algebra
		case ADDL:
			REG(uint64_t, a) += REG(uint64_t, b);
			break;
		case ADDI:
			REG(uint32_t, a) += REG(uint32_t, b);
			break;
		case ADDB:
			REG(uint8_t, a) += REG(uint8_t, b);
			break;
		case MINL:
			REG(uint64_t, a) -= REG(uint64_t, b);
			break;
		case MINI:
			REG(uint32_t, a) -= REG(uint32_t, b);
			break;
		case MINB:
			REG(uint8_t, a) -= REG(uint8_t, b);
			break;
		case MTPL:
			REG(uint64_t, a) *= REG(uint64_t, b);
			break;
		case MTPI:
			REG(uint32_t, a) *= REG(uint32_t, b);
			break;
		case MTPB:
			REG(uint8_t, a) *= REG(uint8_t, b);
			break;
		case DIVL:
			REG(uint64_t, a) /= REG(uint64_t, b);
			break;
		case DIVI:
			REG(uint32_t, a) /= REG(uint32_t, b);
			break;
		case DIVB:
			REG(uint8_t, a) /= REG(uint8_t, b);
			break;
		case MODL:
			REG(uint64_t, a) = REG(uint64_t, a) % REG(uint64_t, b);
			break;
		case MODI:
			REG(uint32_t, a) = REG(uint32_t, b) % REG(uint32_t, b);
			break;
		case MODB:
			REG(uint8_t, a) = REG(uint8_t, b) % REG(uint8_t, b);
			break;
		// ===========================
		//  Logistics & Flow control
		// ===========================
		case ANDL:
			REG(uint64_t, a) &= REG(uint64_t, b);
			break;
		case ANDI:
			REG(uint32_t, a) &= REG(uint32_t, b);
			break;
		case ANDB:
			REG(uint8_t, a) &= REG(uint8_t, b);
			break;
		case OR_L:
			REG(uint64_t, a) |= REG(uint64_t, b);
			break;
		case OR_I:
			REG(uint32_t, a) |= REG(uint32_t, b);
			break;
		case OR_B:
			REG(uint8_t, a) |= REG(uint8_t, b);
			break;
		case NOTL:
			REG(uint64_t, a) = !(REG(uint64_t, a));
			break;
		case NOTI:
			REG(uint32_t, a) = !(REG(uint32_t, a));
			break;
		case NOTB:
			REG(uint8_t, a) = !(REG(uint8_t, a));
			break;
		case XORL:
			REG(uint64_t, a) ^= (REG(uint64_t, a));
			break;
		case XORI:
			REG(uint32_t, a) ^= (REG(uint32_t, a));
			break;
		case XORB:
			REG(uint8_t, a) ^= (REG(uint8_t, a));
			break;
		case CMPL:
			if (REG(uint64_t, a) > REG(uint64_t, b)) {
				*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
			} else if (REG(uint64_t, a) == REG(uint64_t, b)) {
				*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
			} else {
				*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
			}
			break;
		case CMPI:
			if (REG(uint32_t, a) > REG(uint32_t, b)) {
				*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
			} else if (REG(uint32_t, a) == REG(uint32_t, b)) {
				*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
			} else {
				*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
			}
			break;
		case CMPB:
			if (REG(uint8_t, a) > REG(uint8_t, b)) {
				*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
			} else if (REG(uint8_t, a) == REG(uint8_t, b)) {
				*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
			} else {
				*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
			}
			break;
		case JMPR:
			*REG86 = REG(uint32_t, a) - sizeof(Instruct);
			break;
		case JMPA:
			*REG86 = a - sizeof(Instruct);
			break;
		case JI9A:
			if (*REG99 == 0xFF) *REG86 = a - sizeof(Instruct);
			break;
		case JI8A:
			if (*REG98 == 0xFF) *REG86 = a - sizeof(Instruct);
			break;
		case JI7A:
			if (*REG97 == 0xFF) *REG86 = a - sizeof(Instruct);
			break;
		}
	}
	//-------------------------------------------------------------------------
	// ● 输出寄存器值（调试用）
	//-------------------------------------------------------------------------
	void VM::dump_registers() {
		puts("Registers:");
		for (int i = 1; i < 100; i++) {
			printf("%02X%c",
				reg[i],
				i % 10 == 0 ? '\n' : ' '
			);
		}
		printf("\n");
	}
	//-------------------------------------------------------------------------
	// ● 输出内存值（调试用）
	//-------------------------------------------------------------------------
	void VM::dump_memory() {
		puts("Local Memory:");
		for (uint32_t i = 1; i < local_mem_size / 8; i++) {
			printf("%08X%c",
				*((uint32_t*) local_memory + i),
				i % 8 == 0 ? '\n' : ' '
			);
		}
		printf("\n");
	}
}
