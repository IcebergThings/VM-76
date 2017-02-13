//=============================================================================
// ■ ASM76/VM.cpp
//-----------------------------------------------------------------------------
//   The Virtual Machine
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
	//-------------------------------------------------------------------------
	// ● 解释
	//-------------------------------------------------------------------------
	void VM::execute() {
		while ((now = memfetch<Instruct>(*REG86))->opcode != _HLT) {
			printf("%08x: %04x, %x, %x\n", *REG86, now->opcode, now->a, now->b);
			VM::execute_instruction(now);
			*REG86 += sizeof(Instruct);
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
	//-------------------------------------------------------------------------
	// ● 解释……
	//-------------------------------------------------------------------------
	#define execute(x) void VM::execute_##x(uint32_t a, uint32_t b)
	// Unimplemented placeholders
	execute(NOOP) {}
	execute(_HLT) {}
	execute(JI9R) {}
	execute(JI8R) {}
	execute(JI7R) {}
	execute(CALR) {}
	execute(CALA) {}
	execute(RET) {}
	execute(POP) {}
	execute(PUSH) {}
	// ╔═══════════╗
	// ║ □ 76-Base ║
	// ╚═══════════╝
	//-------------------------------------------------------------------------
	// ● LCMM
	//-------------------------------------------------------------------------
	execute(LCMM) {
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
	//-------------------------------------------------------------------------
	// ● LDLA LDIA LDBA
	//-------------------------------------------------------------------------
	execute(LDLA) {
		REG(uint64_t, b) = *memfetch<uint64_t>(a);
	}
	execute(LDIA) {
		REG(uint32_t, b) = *memfetch<uint32_t>(a);
	}
	execute(LDBA) {
		REG(uint8_t, b) = *memfetch<uint8_t>(a);
	}
	//-------------------------------------------------------------------------
	// ● LDLR LDIR LDBR
	//-------------------------------------------------------------------------
	execute(LDLR) {
		REG(uint64_t, b) = *memfetch<uint64_t>(REG(uint32_t, a));
	}
	execute(LDIR) {
		REG(uint32_t, b) = *memfetch<uint32_t>(REG(uint32_t, a));
	}
	execute(LDBR) {
		REG(uint8_t, b) = *memfetch<uint8_t>(REG(uint32_t, a));
	}
	//-------------------------------------------------------------------------
	// ● SLLA SLIA SLBA
	//-------------------------------------------------------------------------
	execute(SLLA) {
		*memfetch<uint64_t>(a) = REG(uint64_t, b);
	}
	execute(SLIA) {
		*memfetch<uint32_t>(a) = REG(uint32_t, b);
	}
	execute(SLBA) {
		*memfetch<uint8_t>(a) = REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● SLLR SLIR SLBR
	//-------------------------------------------------------------------------
	execute(SLLR) {
		*memfetch<uint64_t>(REG(uint32_t, a)) = REG(uint64_t, b);
	}
	execute(SLIR) {
		*memfetch<uint32_t>(REG(uint32_t, a)) = REG(uint32_t, b);
	}
	execute(SLBR) {
		*memfetch<uint8_t>(REG(uint32_t, a)) = REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● DATL DATI DATB
	//-------------------------------------------------------------------------
	execute(DATL) {
		REG(uint64_t, b) = (uint64_t) a;
	}
	execute(DATI) {
		REG(uint32_t, b) = (uint32_t) a;
	}
	execute(DATB) {
		REG(uint8_t, b) = (uint8_t) a;
	}
	//-------------------------------------------------------------------------
	// ● MOVL MOVI MOVB
	//-------------------------------------------------------------------------
	execute(MOVL) {
		*memfetch<uint64_t>(b) = *memfetch<uint64_t>(a);
	}
	execute(MOVI) {
		*memfetch<uint32_t>(b) = *memfetch<uint32_t>(a);
	}
	execute(MOVB) {
		*memfetch<uint8_t>(b) = *memfetch<uint8_t>(a);
	}
	//-------------------------------------------------------------------------
	// ● MVPL MVPI MVPB
	//-------------------------------------------------------------------------
	execute(MVPL) {
		*memfetch<uint64_t>(REG(uint32_t, b)) = *memfetch<uint64_t>(REG(uint32_t, a));
	}
	execute(MVPI) {
		*memfetch<uint32_t>(REG(uint32_t, b)) = *memfetch<uint32_t>(REG(uint32_t, a));
	}
	execute(MVPB) {
		*memfetch<uint8_t>(REG(uint32_t, b)) = *memfetch<uint8_t>(REG(uint32_t, a));
	}
	//-------------------------------------------------------------------------
	// ● MVRL MVRI MVRB
	//-------------------------------------------------------------------------
	execute(MVRL) {
		REG(uint64_t, b) = REG(uint64_t, a);
	}
	execute(MVRI) {
		REG(uint32_t, b) = REG(uint32_t, a);
	}
	execute(MVRB) {
		REG(uint8_t, b) = REG(uint8_t, a);
	}
	//-------------------------------------------------------------------------
	// ● ADDL ADDI ADDB
	//-------------------------------------------------------------------------
	execute(ADDL) {
		REG(uint64_t, a) += REG(uint64_t, b);
	}
	execute(ADDI) {
		REG(uint32_t, a) += REG(uint32_t, b);
	}
	execute(ADDB) {
		REG(uint8_t, a) += REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● MINL MINI MINB
	//-------------------------------------------------------------------------
	execute(MINL) {
		REG(uint64_t, a) -= REG(uint64_t, b);
	}
	execute(MINI) {
		REG(uint32_t, a) -= REG(uint32_t, b);
	}
	execute(MINB) {
		REG(uint8_t, a) -= REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● MTPL MTPI MTPB
	//-------------------------------------------------------------------------
	execute(MTPL) {
		REG(uint64_t, a) *= REG(uint64_t, b);
	}
	execute(MTPI) {
		REG(uint32_t, a) *= REG(uint32_t, b);
	}
	execute(MTPB) {
		REG(uint8_t, a) *= REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● DIVL DIVI DIVB
	//-------------------------------------------------------------------------
	execute(DIVL) {
		REG(uint64_t, a) /= REG(uint64_t, b);
	}
	execute(DIVI) {
		REG(uint32_t, a) /= REG(uint32_t, b);
	}
	execute(DIVB) {
		REG(uint8_t, a) /= REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● MODL MODI MODB
	//-------------------------------------------------------------------------
	execute(MODL) {
		REG(uint64_t, a) = REG(uint64_t, a) % REG(uint64_t, b);
	}
	execute(MODI) {
		REG(uint32_t, a) = REG(uint32_t, b) % REG(uint32_t, b);
	}
	execute(MODB) {
		REG(uint8_t, a) = REG(uint8_t, b) % REG(uint8_t, b);
	}
	// ┌────────────────────────────┐
	// │ □ Logistics & Flow control │
	// └────────────────────────────┘
	//-------------------------------------------------------------------------
	// ● ANDL ANDI ANDB
	//-------------------------------------------------------------------------
	execute(ANDL) {
		REG(uint64_t, a) &= REG(uint64_t, b);
	}
	execute(ANDI) {
		REG(uint32_t, a) &= REG(uint32_t, b);
	}
	execute(ANDB) {
		REG(uint8_t, a) &= REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● OR_L OR_I OR_B
	//-------------------------------------------------------------------------
	execute(OR_L) {
		REG(uint64_t, a) |= REG(uint64_t, b);
	}
	execute(OR_I) {
		REG(uint32_t, a) |= REG(uint32_t, b);
	}
	execute(OR_B) {
		REG(uint8_t, a) |= REG(uint8_t, b);
	}
	//-------------------------------------------------------------------------
	// ● NOTL NOTI NOTB
	//-------------------------------------------------------------------------
	execute(NOTL) {
		REG(uint64_t, a) = !(REG(uint64_t, a));
	}
	execute(NOTI) {
		REG(uint32_t, a) = !(REG(uint32_t, a));
	}
	execute(NOTB) {
		REG(uint8_t, a) = !(REG(uint8_t, a));
	}
	//-------------------------------------------------------------------------
	// ● XORL XORI XORB
	//-------------------------------------------------------------------------
	execute(XORL) {
		REG(uint64_t, a) ^= (REG(uint64_t, a));
	}
	execute(XORI) {
		REG(uint32_t, a) ^= (REG(uint32_t, a));
	}
	execute(XORB) {
		REG(uint8_t, a) ^= (REG(uint8_t, a));
	}
	//-------------------------------------------------------------------------
	// ● CMPL CMPI CMPB
	//-------------------------------------------------------------------------
	execute(CMPL) {
		if (REG(uint64_t, a) > REG(uint64_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint64_t, a) == REG(uint64_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	execute(CMPI) {
		if (REG(uint32_t, a) > REG(uint32_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint32_t, a) == REG(uint32_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	execute(CMPB) {
		if (REG(uint8_t, a) > REG(uint8_t, b)) {
			*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
		} else if (REG(uint8_t, a) == REG(uint8_t, b)) {
			*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
		} else {
			*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
		}
	}
	//-------------------------------------------------------------------------
	// ● JMPR JMPA
	//-------------------------------------------------------------------------
	execute(JMPR) {
		*REG86 = REG(uint32_t, a) - sizeof(Instruct);
	}
	execute(JMPA) {
		*REG86 = a - sizeof(Instruct);
	}
	//-------------------------------------------------------------------------
	// ● JI9A JI8A JI7A
	//-------------------------------------------------------------------------
	execute(JI9A) {
		if (*REG99 == 0xFF) *REG86 = a - sizeof(Instruct);
	}
	execute(JI8A) {
		if (*REG98 == 0xFF) *REG86 = a - sizeof(Instruct);
	}
	execute(JI7A) {
		if (*REG97 == 0xFF) *REG86 = a - sizeof(Instruct);
	}
	// ╔════════════╗
	// ║ □ 76-Float ║
	// ╚════════════╝
	// ╔═════════════╗
	// ║ □ 76-Vector ║
	// ╚═════════════╝
	// ╔═════════════════════╗
	// ║ □ BIOS Instructions ║
	// ╚═════════════════════╝
}
