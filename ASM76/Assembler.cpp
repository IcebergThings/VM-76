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
		size_t buf_size = 10 * sizeof(Instruct);
		r.instruct = (Instruct*) malloc(buf_size);
		while (prg && *prg) switch (*prg) {
		case '#':
			prg = strchr(prg, '\n') + 1;
			break;
		case '\n':
			prg++;
			break;
		default: {
			r.size += sizeof(Instruct);
			if (r.size > buf_size) {
				buf_size += 30 * sizeof(Instruct);
				r.instruct = (Instruct*) realloc(r.instruct, r.size);
			}
			char opcode[13];
			copy_opcode(opcode);
			printf("'%s'\n", opcode);
			Instruct* current_instruct = &r.instruct[r.size / sizeof(Instruct)];
			current_instruct->opcode = parse_opcode(opcode);
			read_parameters(current_instruct);
			r.size += sizeof(Instruct);
		}
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
	// ● 跳过某字符
	//-------------------------------------------------------------------------
	void Assembler::skip(char c) {
		if (*prg != c) {
			char msg[20];
			switch (c) {
			case '\n':
				strcpy(msg, "expected newline");
				break;
			case ' ':
				strcpy(msg, "expected space");
				break;
			default:
				strcpy(msg, "expected '\a'");
				*strchr(msg, '\a') = c;
			}
			error(msg);
		}
		prg++;
	}
	//-------------------------------------------------------------------------
	// ● 复制opcode
	//-------------------------------------------------------------------------
	void Assembler::copy_opcode(char* buf) {
		for (size_t i = 0; i < 12; i++) {
			if (prg[i] == ' ' || prg[i] == '\n') {
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
		#define I(x, ta, tb) if (strcmp(str, #x) == 0) return x;
		#include "instructions.hpp"
		error("unidentified instruction");
		return NOOP;
	}
	//-------------------------------------------------------------------------
	// ● 读取参数
	//-------------------------------------------------------------------------
	void Assembler::read_parameters(Instruct* i) {
		switch (i->opcode) {
		#define TNUL 0
		#define TIMM read_immediate_u32()
		#define TADD read_address()
		#define TREG read_register()
		#define I(x, ta, tb) case x: i->a = ta; i->b = tb; break;
		#include "instructions.hpp"
		}
		skip('\n');
	}
	//-------------------------------------------------------------------------
	// ● 读取立即数参数
	//-------------------------------------------------------------------------
	uint32_t Assembler::read_immediate_u32() {
		skip(' ');
		if (!is_digit(*prg)) error("expected digit");
		long long n = atoll(prg);
		if (n > UINT32_MAX) error("immediate number too large");
		if (n < 0) error("immediate number can't be negative");
		while (is_digit(*prg)) prg++;
		return n;
	}
	//-------------------------------------------------------------------------
	// ● 读取地址参数
	//-------------------------------------------------------------------------
	uint32_t Assembler::read_address() {
		return read_immediate_u32();
	}
	//-------------------------------------------------------------------------
	// ● 读取寄存器参数
	//   返回寄存器编号。
	//-------------------------------------------------------------------------
	uint32_t Assembler::read_register() {
		skip(' ');
		skip('$');
		if (!is_digit(*prg)) error("expected digit");
		int reg = atoi(prg);
		if (reg < 0) error("register no. can't be negative");
		if ((size_t) reg > VM::REGISTER_COUNT) error("register no. too large");
		while (is_digit(*prg)) prg++;
		return reg;
	}
}
