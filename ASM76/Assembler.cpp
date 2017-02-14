//=============================================================================
// ■ Assembler.cpp
//-----------------------------------------------------------------------------
//   ASM76汇编器。
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 {
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	Assembler::Assembler(const char* program) {
		prg = program;
	}
	//-------------------------------------------------------------------------
	// ● 汇编
	//-------------------------------------------------------------------------
	Instruct* assemble() {
		Instruct* r = malloc(sizeof(Instruct) * 10);
		size_t count = 0;
		while (prg && *prg) {
			skip_whitespace();
			if (*prg == '#') {
				skip_line();
				continue;
			}
			char opcode[13];
			get_opcode(opcode);
			r[count].opcode = parse_opcode(opcode);
			r[count].a = 0;
			r[count].b = 0;
		}
		return r;
	}
	//-------------------------------------------------------------------------
	// ● 是否为ASM76中的空白字符？
	//   不使用isspace函数，是因为这里的空白和它的空白定义不太相同。
	//-------------------------------------------------------------------------
	bool is_space(char c) {
		return strchr(SPACE, (unsigned char) c) != NULL;
	}
	//-------------------------------------------------------------------------
	// ● 跳过空白
	//-------------------------------------------------------------------------
	void skip_whitespace() {
		prg += strspn(prg, SPACE);
	}
	//-------------------------------------------------------------------------
	// ● 跳过行
	//-------------------------------------------------------------------------
	void skip_line() {
		prg = strchr(prg, '\n') + 1;
	}
	//-------------------------------------------------------------------------
	// ● 复制opcode
	//-------------------------------------------------------------------------
	bool get_opcode(char* buf) {
		const char* first_space = prg;
		for (size_t i = 0; i < 12; i++) {
			if (is_space(prg[i]) || prg[i] == '\n') {
				memcpy(opcode, prg, i);
				opcode[i] = 0;
				return true;
			}
		}
		printf("opcode too long");
		return false;
	}
	//-------------------------------------------------------------------------
	// ● opcode: char[] → enum InstructionOpcode
	//-------------------------------------------------------------------------
	enum InstructionOpcode parse_opcode(const char* str) {
		#define I(x) if (strcmp(str, #x) == 0) return X;
			#include "instructions.hpp"
		#undef I
	}
}
