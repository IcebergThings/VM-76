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
		switch (i->opcode) {
		#define I(x) case x: execute_##x(i->a, i->b); break;
			#include "instructions.hpp"
		#undef I
		default:
			printf("Unknown opcode %d (0x%x)\n", i->opcode);
		}
	}
	// ===========================
	//  76-Base
	// ===========================
	// LCMM
	void VM::execute_LCMM(uint32_t a, uint32_t b) {
		uint8_t* new_mem = new uint8_t[a];
		memset(new_mem, 0, a);
		size_t copied_size = a < local_mem_size ? a : local_mem_size;
		memcpy(new_mem, local_memory, copied_size);
		free(local_memory);
		local_mem_size = a;
		local_memory = new_mem;
		instruct_memory = (Instruct*) new_mem;
		printf("Changed local size to %zu bytes\n", local_mem_size);
	}
	// Load Data
	void VM::execute_LDLA(uint32_t a, uint32_t b) {
		REG(uint64_t, b) = *memfetch<uint64_t>(a);
	}
	void VM::execute_LDIA(uint32_t a, uint32_t b) {
		REG(uint32_t, b) = *memfetch<uint32_t>(a);
	}
	void VM::execute_LDBA(uint32_t a, uint32_t b) {
		REG(uint8_t, b) = *memfetch<uint8_t>(a);
	}
	void VM::execute_LDLR(uint32_t a, uint32_t b) {
		REG(uint64_t, b) = *memfetch<uint64_t>(REG(uint32_t, a));
	}
	void VM::execute_LDIR(uint32_t a, uint32_t b) {
		REG(uint32_t, b) = *memfetch<uint32_t>(REG(uint32_t, a));
	}
	void VM::execute_LDBR(uint32_t a, uint32_t b) {
		REG(uint8_t, b) = *memfetch<uint8_t>(REG(uint32_t, a));
	}
	// Store data
	void VM::execute_SLLA(uint32_t a, uint32_t b) {
		*memfetch<uint64_t>(a) = REG(uint64_t, b);
	}
	void VM::execute_SLIA(uint32_t a, uint32_t b) {
		*memfetch<uint32_t>(a) = REG(uint32_t, b);
	}
	void VM::execute_SLBA(uint32_t a, uint32_t b) {
		*memfetch<uint8_t>(a) = REG(uint8_t, b);
	}
	void VM::execute_SLLR(uint32_t a, uint32_t b) {
		*memfetch<uint64_t>(REG(uint32_t, a)) = REG(uint64_t, b);
	}
	void VM::execute_SLIR(uint32_t a, uint32_t b) {
		*memfetch<uint32_t>(REG(uint32_t, a)) = REG(uint32_t, b);
	}
	void VM::execute_SLBR(uint32_t a, uint32_t b) {
		*memfetch<uint8_t>(REG(uint32_t, a)) = REG(uint8_t, b);
	}
	void VM::execute_DATL(uint32_t a, uint32_t b) {
		REG(uint64_t, b) = (uint64_t) a;
	}
	void VM::execute_DATI(uint32_t a, uint32_t b) {
		REG(uint32_t, b) = (uint32_t) a;
	}
	void VM::execute_DATB(uint32_t a, uint32_t b) {
		REG(uint8_t, b) = (uint8_t) a;
	}
	// Mem operation
	void VM::execute_MOVL(uint32_t a, uint32_t b) {
		*memfetch<uint64_t>(b) = *memfetch<uint64_t>(a);
	}
	void VM::execute_MOVI(uint32_t a, uint32_t b) {
		*memfetch<uint32_t>(b) = *memfetch<uint32_t>(a);
	}
	void VM::execute_MOVB(uint32_t a, uint32_t b) {
		*memfetch<uint8_t>(b) = *memfetch<uint8_t>(a);
	}
	void VM::execute_MVPL(uint32_t a, uint32_t b) {
		*memfetch<uint64_t>(REG(uint32_t, b)) = *memfetch<uint64_t>(REG(uint32_t, a));
	}
	void VM::execute_MVPI(uint32_t a, uint32_t b) {
		*memfetch<uint32_t>(REG(uint32_t, b)) = *memfetch<uint32_t>(REG(uint32_t, a));
	}
	void VM::execute_MVPB(uint32_t a, uint32_t b) {
		*memfetch<uint8_t>(REG(uint32_t, b)) = *memfetch<uint8_t>(REG(uint32_t, a));
	}
	void VM::execute_MVRL(uint32_t a, uint32_t b) {
		REG(uint64_t, b) = REG(uint64_t, a);
	}
	void VM::execute_MVRI(uint32_t a, uint32_t b) {
		REG(uint32_t, b) = REG(uint32_t, a);
	}
	void VM::execute_MVRB(uint32_t a, uint32_t b) {
		REG(uint8_t, b) = REG(uint8_t, a);
	}
	// Basic Algebra
	void VM::execute_ADDL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) += REG(uint64_t, b);
	}
	void VM::execute_ADDI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) += REG(uint32_t, b);
	}
	void VM::execute_ADDB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) += REG(uint8_t, b);
	}
	void VM::execute_MINL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) -= REG(uint64_t, b);
	}
	void VM::execute_MINI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) -= REG(uint32_t, b);
	}
	void VM::execute_MINB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) -= REG(uint8_t, b);
	}
	void VM::execute_MTPL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) *= REG(uint64_t, b);
	}
	void VM::execute_MTPI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) *= REG(uint32_t, b);
	}
	void VM::execute_MTPB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) *= REG(uint8_t, b);
	}
	void VM::execute_DIVL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) /= REG(uint64_t, b);
	}
	void VM::execute_DIVI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) /= REG(uint32_t, b);
	}
	void VM::execute_DIVB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) /= REG(uint8_t, b);
	}
	void VM::execute_MODL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) = REG(uint64_t, a) % REG(uint64_t, b);
	}
	void VM::execute_MODI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) = REG(uint32_t, b) % REG(uint32_t, b);
	}
	void VM::execute_MODB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) = REG(uint8_t, b) % REG(uint8_t, b);
	}
	// ===========================
	//  Logistics & Flow control
	// ===========================
	void VM::execute_ANDL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) &= REG(uint64_t, b);
	}
	void VM::execute_ANDI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) &= REG(uint32_t, b);
	}
	void VM::execute_ANDB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) &= REG(uint8_t, b);
	}
	void VM::execute_OR_L(uint32_t a, uint32_t b) {
		REG(uint64_t, a) |= REG(uint64_t, b);
	}
	void VM::execute_OR_I(uint32_t a, uint32_t b) {
		REG(uint32_t, a) |= REG(uint32_t, b);
	}
	void VM::execute_OR_B(uint32_t a, uint32_t b) {
		REG(uint8_t, a) |= REG(uint8_t, b);
	}
	void VM::execute_NOTL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) = !(REG(uint64_t, a));
	}
	void VM::execute_NOTI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) = !(REG(uint32_t, a));
	}
	void VM::execute_NOTB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) = !(REG(uint8_t, a));
	}
	void VM::execute_XORL(uint32_t a, uint32_t b) {
		REG(uint64_t, a) ^= (REG(uint64_t, a));
	}
	void VM::execute_XORI(uint32_t a, uint32_t b) {
		REG(uint32_t, a) ^= (REG(uint32_t, a));
	}
	void VM::execute_XORB(uint32_t a, uint32_t b) {
		REG(uint8_t, a) ^= (REG(uint8_t, a));
	}
	void VM::execute_CMPL(uint32_t a, uint32_t b) {
		if (REG(uint64_t, a) > REG(uint64_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint64_t, a) == REG(uint64_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	void VM::execute_CMPI(uint32_t a, uint32_t b) {
		if (REG(uint32_t, a) > REG(uint32_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint32_t, a) == REG(uint32_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	void VM::execute_CMPB(uint32_t a, uint32_t b) {
		if (REG(uint8_t, a) > REG(uint8_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint8_t, a) == REG(uint8_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	void VM::execute_JMPR(uint32_t a, uint32_t b) {
		*REG86 = REG(uint32_t, a) - sizeof(Instruct);
	}
	void VM::execute_JMPA(uint32_t a, uint32_t b) {
		*REG86 = a - sizeof(Instruct);
	}
	void VM::execute_JI9A(uint32_t a, uint32_t b) {
		if (*REG99 == 0xFF) *REG86 = a - sizeof(Instruct);
	}
	void VM::execute_JI8A(uint32_t a, uint32_t b) {
		if (*REG98 == 0xFF) *REG86 = a - sizeof(Instruct);
	}
	void VM::execute_JI7A(uint32_t a, uint32_t b) {
		if (*REG97 == 0xFF) *REG86 = a - sizeof(Instruct);
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
