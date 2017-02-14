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
	Program Assembler::assemble() {
		Program r;
		r.size = 0;
		size_t current_size = 10 * sizeof(Instruct);
		r.instruct = (Instruct*) malloc(current_size);
		while (prg && *prg) {
			skip_whitespace(false);
			if (*prg == '#') {
				skip_whole_line();
				continue;
			}
			if (*prg == '\n') {
				prg++;
				continue;
			}
			current_size += sizeof(Instruct);
			if (current_size > r.size) {
				r.size += 30 * sizeof(Instruct);
				r.instruct = (Instruct*) realloc(r.instruct, r.size);
			}
			char opcode[13];
			get_opcode(opcode);
			r.instruct[current_size / sizeof(Instruct)].opcode = parse_opcode(opcode);
			read_parameters(&r.instruct[current_size / sizeof(Instruct)]);
			current_size += sizeof(Instruct);
		}
		return r;
	}
	//-------------------------------------------------------------------------
	// ● 报错！
	//-------------------------------------------------------------------------
	void Assembler::error(const char* message) {
		printf("Error: %s\nAssembly:\n%s\n", message, prg);
		abort();
	}
	//-------------------------------------------------------------------------
	// ● 是否为ASM76中的空白字符？
	//   不使用isspace函数，是因为这里的空白和它的空白定义不太相同。
	//-------------------------------------------------------------------------
	bool Assembler::is_space(char c) {
		return strchr(SPACE, (unsigned char) c) != NULL;
	}
	//-------------------------------------------------------------------------
	// ● 是否为十进制数字字符？
	//-------------------------------------------------------------------------
	bool Assembler::is_digit(char c) {
		return c >= '0' && c <= '9';
	}
	//-------------------------------------------------------------------------
	// ● 读取单个十进制数字字符
	//-------------------------------------------------------------------------
	int Assembler::read_digit() {
		if (!is_digit(*prg)) error("expected digit");
		return *prg++ - '0';
	}
	//-------------------------------------------------------------------------
	// ● 是否为十六进制数字字符？
	//-------------------------------------------------------------------------
	bool Assembler::is_xdigit(char c) {
		return isxdigit((unsigned char) c);
	}
	//-------------------------------------------------------------------------
	// ● 读取单个十六进制数字字符
	//-------------------------------------------------------------------------
	int Assembler::read_xdigit() {
		if (!is_xdigit(*prg)) error("expected hexadecimal digit");
		if (is_digit(*prg)) return read_digit();
		if (*prg >= 'a') return *prg++ - 'a' + 10;
		return *prg++ - 'A' + 10;
	}
	//-------------------------------------------------------------------------
	// ● 跳过空白
	//-------------------------------------------------------------------------
	void Assembler::skip_whitespace(bool required) {
		size_t len = strspn(prg, SPACE);
		if (required && !len) error("expected whitespace");
		prg += len;
	}
	//-------------------------------------------------------------------------
	// ● 跳过整行
	//-------------------------------------------------------------------------
	void Assembler::skip_whole_line() {
		prg = strchr(prg, '\n') + 1;
	}
	//-------------------------------------------------------------------------
	// ● 跳过行
	//-------------------------------------------------------------------------
	void Assembler::skip_line() {
		skip_whitespace(false);
		if (*prg != '\n') error("expected newline");
	}
	//-------------------------------------------------------------------------
	// ● 复制opcode
	//-------------------------------------------------------------------------
	void Assembler::get_opcode(char* buf) {
		for (size_t i = 0; i < 12; i++) {
			if (is_space(prg[i]) || prg[i] == '\n') {
				memcpy(buf, prg, i);
				buf[i] = 0;
				prg += i;
				return;
			}
		}
		error("opcode too long");
	}
	//-------------------------------------------------------------------------
	// ● opcode: char[] → enum InstructionOpcode
	//-------------------------------------------------------------------------
	enum InstructionOpcode Assembler::parse_opcode(const char* str) {
		#define I(x) if (strcmp(str, #x) == 0) return x;
			#include "instructions.hpp"
		#undef I
		error("unidentified instruction");
		return NOOP;
	}
	//-------------------------------------------------------------------------
	// ● 读取参数
	//-------------------------------------------------------------------------
	void Assembler::read_parameters(Instruct* i) {
		i->a = 0;
		i->b = 0;
		switch (i->opcode) {
		// No parameters
		case NOOP:
		case HALT:
		case RETN:
			break;
		// 1 parameter: direct number
		case LCMM:
			skip_whitespace(true);
			break;
		// 1 parameter: register
		case NOTL: case NOTI: case NOTB:
			skip_whitespace(true);
			break;
		// 2 parameters
		case DATL: case DATI: case DATB:
			skip_whitespace(true);
			break;
		}
	}
	//-------------------------------------------------------------------------
	// ● 读取寄存器参数
	//   返回寄存器编号。
	//-------------------------------------------------------------------------
	int Assembler::read_register() {
		if (*prg != '$') error("expected register name");
		prg++;
		int reg = read_digit();
		for (size_t i = 0; i < 2; i++) {
			if (is_digit(*prg)) {
				reg = reg * 10 + read_digit();
			} else {
				return reg;
			}
		}
		if (is_digit(*prg)) error("register number too long");
		return -1;
	}
}
