//=============================================================================
// ■ ASM76/VM.cpp
//-----------------------------------------------------------------------------
//   Virtual Machine
//=============================================================================

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

		REG86 = (uint32_t*) (reg + 86);
		REG90 = (uint32_t*) (reg + 90);
		REG97 = (uint8_t*) (reg + 97);
		REG98 = (uint8_t*) (reg + 98);
		REG99 = (uint8_t*) (reg + 99);
	}

	#define OPC(code) (now->opcode == code)

	void VM::execute() {
		Instruct* now = memfetch<Instruct>(*REG86);
		while (!OPC(_HLT)) {
			printf("%08x : %04x, %x, %x\n", *REG86, now->opcode, now->f, now->t);

			// ===========================
			//  76-Base
			// ===========================
			// LCMM
			if OPC(LCMM) {
				uint8_t* new_mem = new uint8_t[now->f];
				memset(new_mem, 0, now->f);
				size_t copied_size = now->f < local_mem_size ? now->f : local_mem_size;
				memcpy(new_mem, local_memory, copied_size);
				free(local_memory);
				local_mem_size = now->f;
				local_memory = new_mem;
				instruct_memory = (Instruct*) new_mem;

				printf("Changed local size to %zu bytes\n", local_mem_size);
			}
			// Load Data
			else if OPC(LDLA) {
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
			}
			// Store data
			else if OPC(SLLA) {
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
			} else if OPC(DATL) {
				REG(uint64_t, now->t) = (uint64_t) now->f;
			} else if OPC(DATI) {
				REG(uint32_t, now->t) = (uint32_t) now->f;
			} else if OPC(DATB) {
				REG(uint8_t, now->t) = (uint8_t) now->f;
			}
			// Mem operation
			else if OPC(MOVL) {
				*memfetch<uint64_t>(now->t) = *memfetch<uint64_t>(now->f);
			} else if OPC(MOVI) {
				*memfetch<uint32_t>(now->t) = *memfetch<uint32_t>(now->f);
			} else if OPC(MOVB) {
				*memfetch<uint8_t>(now->t) = *memfetch<uint8_t>(now->f);
			} else if OPC(MVPL) {
				*memfetch<uint64_t>(REG(uint32_t, now->t)) = *memfetch<uint64_t>(REG(uint32_t, now->f));
			} else if OPC(MVPI) {
				*memfetch<uint32_t>(REG(uint32_t, now->t)) = *memfetch<uint32_t>(REG(uint32_t, now->f));
			} else if OPC(MVPB) {
				*memfetch<uint8_t>(REG(uint32_t, now->t)) = *memfetch<uint8_t>(REG(uint32_t, now->f));
			} else if OPC(MVRL) {
				REG(uint64_t, now->t) = REG(uint64_t, now->f);
			} else if OPC(MVRI) {
				REG(uint32_t, now->t) = REG(uint32_t, now->f);
			} else if OPC(MVRB) {
				REG(uint8_t, now->t) = REG(uint8_t, now->f);
			}
			// Basic Algebra
			else if OPC(ADDL) {
				REG(uint64_t, now->f) += REG(uint64_t, now->t);
			} else if OPC(ADDI) {
				REG(uint32_t, now->f) += REG(uint32_t, now->t);
			} else if OPC(ADDB) {
				REG(uint8_t, now->f) += REG(uint8_t, now->t);
			} else if OPC(MINL) {
				REG(uint64_t, now->f) -= REG(uint64_t, now->t);
			} else if OPC(MINI) {
				REG(uint32_t, now->f) -= REG(uint32_t, now->t);
			} else if OPC(MINB) {
				REG(uint8_t, now->f) -= REG(uint8_t, now->t);
			} else if OPC(MTPL) {
				REG(uint64_t, now->f) *= REG(uint64_t, now->t);
			} else if OPC(MTPI) {
				REG(uint32_t, now->f) *= REG(uint32_t, now->t);
			} else if OPC(MTPB) {
				REG(uint8_t, now->f) *= REG(uint8_t, now->t);
			} else if OPC(DIVL) {
				REG(uint64_t, now->f) /= REG(uint64_t, now->t);
			} else if OPC(DIVI) {
				REG(uint32_t, now->f) /= REG(uint32_t, now->t);
			} else if OPC(DIVB) {
				REG(uint8_t, now->f) /= REG(uint8_t, now->t);
			} else if OPC(MODL) {
				REG(uint64_t, now->f) = REG(uint64_t, now->f) % REG(uint64_t, now->t);
			} else if OPC(MODI) {
				REG(uint32_t, now->f) = REG(uint32_t, now->t) % REG(uint32_t, now->t);
			} else if OPC(MODB) {
				REG(uint8_t, now->f) = REG(uint8_t, now->t) % REG(uint8_t, now->t);
			}
			// ===========================
			//  Logistics & Flow control
			// ===========================
			if (now->opcode >= ANDL) {
				if OPC(ANDL) {
					REG(uint64_t, now->f) &= REG(uint64_t, now->t);
				} else if OPC(ANDI) {
					REG(uint32_t, now->f) &= REG(uint32_t, now->t);
				} else if OPC(ANDB) {
					REG(uint8_t, now->f) &= REG(uint8_t, now->t);
				} else if OPC(OR_L) {
					REG(uint64_t, now->f) |= REG(uint64_t, now->t);
				} else if OPC(OR_I) {
					REG(uint32_t, now->f) |= REG(uint32_t, now->t);
				} else if OPC(OR_B) {
					REG(uint8_t, now->f) |= REG(uint8_t, now->t);
				} else if OPC(NOTL) {
					REG(uint64_t, now->f) = !(REG(uint64_t, now->f));
				} else if OPC(NOTI) {
					REG(uint32_t, now->f) = !(REG(uint32_t, now->f));
				} else if OPC(NOTB) {
					REG(uint8_t, now->f) = !(REG(uint8_t, now->f));
				} else if OPC(XORL) {
					REG(uint64_t, now->f) ^= (REG(uint64_t, now->f));
				} else if OPC(XORI) {
					REG(uint32_t, now->f) ^= (REG(uint32_t, now->f));
				} else if OPC(XORB) {
					REG(uint8_t, now->f) ^= (REG(uint8_t, now->f));
				} else if OPC(CMPL) {
					if (REG(uint64_t, now->f) > REG(uint64_t, now->t)) {
						*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
					} else if (REG(uint64_t, now->f) == REG(uint64_t, now->t)) {
						*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
					} else {
						*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
					}
				} else if OPC(CMPI) {
					if (REG(uint32_t, now->f) > REG(uint32_t, now->t)) {
						*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
					} else if (REG(uint32_t, now->f) == REG(uint32_t, now->t)) {
						*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
					} else {
						*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
					}
				} else if OPC(CMPB) {
					if (REG(uint8_t, now->f) > REG(uint8_t, now->t)) {
						*REG99 = 0xFF; *REG98 = 0x0; *REG97 = 0x0;
					} else if (REG(uint8_t, now->f) == REG(uint8_t, now->t)) {
						*REG99 = 0x0; *REG98 = 0xFF; *REG97 = 0x0;
					} else {
						*REG99 = 0x0; *REG98 = 0x0; *REG97 = 0xFF;
					}
				} else if OPC(JMPR) {
					*REG86 = REG(uint32_t, now->f) - sizeof(Instruct);
				} else if OPC(JMPA) {
					*REG86 = now->f - sizeof(Instruct);
				} else if OPC(JI9A) {
					if (*REG99 == 0xFF)
						*REG86 = now->f - sizeof(Instruct);
				} else if OPC(JI8A) {
					if (*REG98 == 0xFF)
						*REG86 = now->f - sizeof(Instruct);
				} else if OPC(JI7A) {
					if (*REG97 == 0xFF)
						*REG86 = now->f - sizeof(Instruct);
				} else if OPC(JI9R) {
					if (*REG99 == 0xFF)
						*REG86 = REG(uint32_t, now->f) - sizeof(Instruct);
				} else if OPC(JI8R) {
					if (*REG98 == 0xFF)
						*REG86 = REG(uint32_t, now->f) - sizeof(Instruct);
				} else if OPC(JI7R) {
					if (*REG97 == 0xFF)
						*REG86 = REG(uint32_t, now->f) - sizeof(Instruct);
				} else if OPC(CALA) {
					*memfetch<uint32_t>(*REG90) = *REG86;
					*REG90 += sizeof(uint32_t);
					*REG86 = now->f - sizeof(Instruct);
				} else if OPC(CALR) {
					*memfetch<uint32_t>(*REG90) = *REG86;
					*REG90 += sizeof(uint32_t);
					*REG86 = REG(uint32_t, now->f) - sizeof(Instruct);
				} else if OPC(RET) {
					*REG86 = *memfetch<uint32_t>(*REG90);
					*REG90 -= sizeof(uint32_t);
				} else if OPC(PUSH) {
					for (uint8_t i = now->f; i < now->f + now->t; i++) {
						*memfetch<uint8_t>(*REG90) = REG(uint8_t, i);
						*REG90 += sizeof(uint8_t);
					}
				} else if OPC(POP) {
					for (uint8_t i = now->f + now->t - 1; i > now->f; i++) {
						REG(uint8_t, i) = *memfetch<uint8_t>(*REG90);
						*REG90 -= sizeof(uint8_t);
					}
				}
			}

			*REG86 += sizeof(Instruct);
			now = memfetch<Instruct>(*REG86);
		}
	}

	void VM::dump_registers() {
		printf("Registers: \n");
		for (int i = 1; i < 100; i++) {
			printf("%02x ", reg[i]);
			if (i % 10 == 0) printf("\n");
		}
		printf("\n");
		printf("Instruction Pointer: %08x\n", *REG86);
		printf("Stack Pointer: %08x\n", *REG90);
	}

	void VM::dump_memory() {
		printf("Local Memory: \n");
		for (uint32_t i = 1; i < local_mem_size / 8; i++) {
			printf("%08x ", *((uint32_t*)local_memory + i));
			if (i % 8 == 0) printf("\n");
		}
		printf("\n");
	}

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
