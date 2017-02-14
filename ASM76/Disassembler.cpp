//=============================================================================
// ■ Disassembler.cpp
//-----------------------------------------------------------------------------
//   ASM76反汇编器。
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 {
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	Disassembler::Disassembler(const Program p) {
		prg = p;
	}
	//-------------------------------------------------------------------------
	// ● 反汇编
	//-------------------------------------------------------------------------
	char* Disassembler::disassemble() {
		size_t buf_size = 100;
		size_t current_size = 1;
		char* r = (char*) malloc(buf_size);
		*r = 0;
		for (size_t i = 0; i < prg.size / sizeof(Instruct); i++) {
			char line[100];
			auto opcode = (InstructionOpcode) prg.instruct[i].opcode;
			strcpy(line, get_opcode_name(opcode));
			switch (opcode) {
			case NOOP:
			case HALT:
				break;
			}
			printf("%s %d %d\n", get_opcode_name(opcode), prg.instruct[i].a, prg.instruct[i].b);
			strcat(line, "\n");
			size_t len = strlen(line);
			current_size += len;
			if (current_size > buf_size) {
				buf_size += 500;
				r = (char*) realloc(r, buf_size);
			}
			strcat(r, line);
		}
		return r;
	}
	//-------------------------------------------------------------------------
	// ● opcode → const char*
	//-------------------------------------------------------------------------
	const char* Disassembler::get_opcode_name(enum InstructionOpcode opcode) {
		switch (opcode) {
		#define I(x) case x: return #x;
			#include "instructions.hpp"
		#undef I
		default:
			return "????";
		}
	}
}
