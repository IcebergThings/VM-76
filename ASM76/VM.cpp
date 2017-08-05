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
	VM::VM(Program program) {
		// 16K local memory in default
		// use malloc series for realloc
		local_memory = (uint8_t*) calloc(sizeof(uint8_t), local_memory_size);
		instruct_memory = (Instruct*) local_memory;
		memcpy(instruct_memory, program.instruct, program.size);

		// 100+ registers
		reg = new uint8_t[REGISTER_COUNT];
		memset(reg, 0, REGISTER_COUNT);

		// Setup registers
		// Instruction pointer
		REG100 = 0x0;
		// Stack bottom pointer
		REG104 = 0x3000;
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	VM::~VM() {
		free(local_memory);
		delete[] reg;
	}
	//-------------------------------------------------------------------------
	// ● 输出寄存器值（调试用）
	//-------------------------------------------------------------------------
	void VM::dump_registers() {
		puts("Registers:");
		for (size_t i = 0; i < REGISTER_COUNT; i++) {
			printf("[%03zu] = %02X,%c",
				i,
				reg[i],
				i % 10 == 9 ? '\n' : ' '
			);
		}
		printf("\n");
		printf("Instruction Pointer: %08x\n", REG100);
		printf("Stack Pointer: %08x\n", REG104);
	}
	//-------------------------------------------------------------------------
	// ● 输出内存值（调试用）
	//-------------------------------------------------------------------------
	void VM::dump_memory() {
		puts("Local Memory:");
		for (uint32_t i = 1; i < local_memory_size / 8; i++) {
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
	void VM::execute_from(uint32_t start_pos, bool debug_process) {
		REG100 = start_pos;
		Instruct* now = memfetch<Instruct>(REG100);
		uint16_t opcode = now->opcode;
		while (opcode != HALT) {
			if (debug_process) printf("%08x: %04x, %x, %x\n", REG100, now->opcode, now->a, now->b);
			VM::execute_instruction_inline(opcode, now->a, now->b);
			REG100 += sizeof(Instruct);
			now = memfetch<Instruct>(REG100);
			opcode = now->opcode;
		}
	}
	//-------------------------------------------------------------------------
	// ● 解释
	//-------------------------------------------------------------------------
	void VM::execute(bool debug_process) {
		Instruct* now = memfetch<Instruct>(REG100);
		uint16_t opcode = now->opcode;
		while (opcode != HALT) {
			if (debug_process) printf("%08x: %04x, %x, %x\n", REG100, now->opcode, now->a, now->b);
			VM::execute_instruction_inline(opcode, now->a, now->b);
			REG100 += sizeof(Instruct);
			now = memfetch<Instruct>(REG100);
			opcode = now->opcode;
		}
	}
	//-------------------------------------------------------------------------
	// ● 解释一条指令 - 模板
	//-------------------------------------------------------------------------
	inline void VM::execute_instruction_inline(uint16_t opcode, uint32_t a, uint32_t b) {
		switch (opcode) {
		#define I(x, ta, tb) case x: execute_##x(a, b); break;
		#include "instructions.hpp"
		default:
			printf("Unknown opcode %d (0x%x)\n", opcode, opcode);
		}
	}
	//-------------------------------------------------------------------------
	// ● 解释一条指令
	//-------------------------------------------------------------------------
	void VM::execute_instruction(Instruct* i) {
		execute_instruction_inline(i->opcode, i->a, i->b);
	}
	//-------------------------------------------------------------------------
	// ● 解释……
	//-------------------------------------------------------------------------
	#define execute(x) inline void VM::execute_##x(uint32_t a, uint32_t b)
	// ╔═══════════╗
	// ║ □ 76-Base ║
	// ╚═══════════╝
	//-------------------------------------------------------------------------
	// ● NOOP HALT
	//-------------------------------------------------------------------------
	execute(NOOP) {}
	execute(HALT) {}
	//-------------------------------------------------------------------------
	// ● LCMM
	//-------------------------------------------------------------------------
	execute(LCMM) {
		uint8_t* new_mem = new uint8_t[a];
		memset(new_mem, 0, a);
		size_t copied_size = a < local_memory_size ? a : local_memory_size;
		memcpy(new_mem, local_memory, copied_size);
		free(local_memory);
		local_memory_size = a;
		local_memory = new_mem;
		instruct_memory = (Instruct*) new_mem;
		printf("Changed local size to %zu bytes\n", local_memory_size);
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
		REG(uint64_t, b) = (uint64_t) a << 32 | b;
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
		if (REG(uint64_t, a) > REG(uint64_t, b))
			REGCMP = 0x2;
		else if (REG(uint64_t, a) == REG(uint64_t, b))
			REGCMP = 0x1;
		else
			REGCMP = 0x0;
	}
	execute(CMPI) {
		if (REG(uint32_t, a) > REG(uint32_t, b))
			REGCMP = 0x2;
		else if (REG(uint32_t, a) == REG(uint32_t, b))
			REGCMP = 0x1;
		else
			REGCMP = 0x0;
	}
	execute(CMPB) {
		if (REG(uint8_t, a) > REG(uint8_t, b))
			REGCMP = 0x2;
		else if (REG(uint8_t, a) == REG(uint8_t, b))
			REGCMP = 0x1;
		else
			REGCMP = 0x0;
	}
	//-------------------------------------------------------------------------
	// ● JMPR JMPA
	//-------------------------------------------------------------------------
	execute(JMPR) {
		REG100 = REG(uint32_t, a) - sizeof(Instruct);
	}
	execute(JMPA) {
		REG100 = a - sizeof(Instruct);
	}
	//-------------------------------------------------------------------------
	// ● JIGA JIEA JILA
	//-------------------------------------------------------------------------
	execute(JIGA) {
		if (REGCMP == 0x2) REG100 = a - sizeof(Instruct);
	}
	execute(JIEA) {
		if (REGCMP == 0x1) REG100 = a - sizeof(Instruct);
	}
	execute(JILA) {
		if (REGCMP == 0x0) REG100 = a - sizeof(Instruct);
	}
	//-------------------------------------------------------------------------
	// ● JIGR JIER JILR
	//-------------------------------------------------------------------------
	execute(JIGR) {
		if (REGCMP == 0x2) REG100 = REG(uint32_t, a) - sizeof(Instruct);
	}
	execute(JIER) {
		if (REGCMP == 0x1) REG100 = REG(uint32_t, a) - sizeof(Instruct);
	}
	execute(JILR) {
		if (REGCMP == 0x0) REG100 = REG(uint32_t, a) - sizeof(Instruct);
	}
	//-------------------------------------------------------------------------
	// ● CALA CALR
	//-------------------------------------------------------------------------
	execute(CALA) {
		*memfetch<uint32_t>(REG104) = REG100;
		REG104 += sizeof(uint32_t);
		REG100 = a - sizeof(Instruct);
	}
	execute(CALR) {
		*memfetch<uint32_t>(REG104) = REG100;
		REG104 += sizeof(uint32_t);
		REG100 = REG(uint32_t, a) - sizeof(Instruct);
	}
	//-------------------------------------------------------------------------
	// ● RETN
	//-------------------------------------------------------------------------
	execute(RETN) {
		REG100 = *memfetch<uint32_t>(REG104);
		REG104 -= sizeof(uint32_t);
	}
	//-------------------------------------------------------------------------
	// ● PUSH
	//-------------------------------------------------------------------------
	execute(PUSH) {
		for (uint8_t i = a; i < a + b; i++) {
			*memfetch<uint8_t>(REG104) = REG(uint8_t, i);
			REG104 += sizeof(uint8_t);
		}
	}
	//-------------------------------------------------------------------------
	// ● POP_
	//-------------------------------------------------------------------------
	execute(POP_) {
		for (uint8_t i = a + b - 1; i > a; i++) {
			REG(uint8_t, i) = *memfetch<uint8_t>(REG104);
			REG104 -= sizeof(uint8_t);
		}
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
	//-------------------------------------------------------------------------
	// ● INTX
	//-------------------------------------------------------------------------
	execute(INTX) {
		REG(uint32_t, 0) = firmware->call(a, memfetch<uint8_t>(b));
	}
	//-------------------------------------------------------------------------
	// ● INTR
	//-------------------------------------------------------------------------
	execute(INTR) {
		REG(uint32_t, 0) = firmware->call(REG(uint8_t, a), memfetch<uint8_t>(b));
	}
}
