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
			uint32_t a = prg.instruct[i].a;
			uint32_t b = prg.instruct[i].b;
			switch (opcode) {
			case NOOP: case HALT: case RETN:
				break;
			case JMPA: case JI7A: case JI8A: case JI9A: case CALA:
				append_address(line, a);
				break;
			case JMPR: case JI7R: case JI8R: case JI9R: case CALR:
				append_register(line, a);
				break;
			case PUSH: case POP_:
				append_register(line, a);
				append_number(line, b);
				break;
			}
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
	//-------------------------------------------------------------------------
	// ● 附加立即数
	//-------------------------------------------------------------------------
	void Disassembler::append_number(char* line, uint32_t x) {
		sprintf(strchr(line, 0), " %d", x);
	}
	//-------------------------------------------------------------------------
	// ● 附加寄存器
	//-------------------------------------------------------------------------
	void Disassembler::append_register(char* line, uint32_t r) {
		sprintf(strchr(line, 0), " $%d", r);
	}
	//-------------------------------------------------------------------------
	// ● 附加地址
	//-------------------------------------------------------------------------
	void Disassembler::append_address(char* line, uint32_t a) {
		sprintf(strchr(line, 0), " 0x%08x", a);
	}
}
