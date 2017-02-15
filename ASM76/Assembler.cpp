//=============================================================================
// ■ Assembler.cpp
//-----------------------------------------------------------------------------
//   ASM76汇编器。
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 {
	#define SPACE " \t\v"
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
		// 实际已用0字节
		r.size = 0;
		// 预留10个指令空位
		size_t buf_size = 10 * sizeof(Instruct);
		r.instruct = (Instruct*) malloc(buf_size);
		// 这个指针指向当前正要填充的指令
		Instruct* current_instruct = r.instruct;
		while (prg && *prg) switch (*prg) {
		case '#':
			prg = strchr(prg, '\n') + 1;
			break;
		case '\n':
			prg++;
			break;
		default: {
			// 需要添加指令但预留空间不足时，
			if (r.size > buf_size) {
				// 补充30个指令空位。
				buf_size += 30 * sizeof(Instruct);
				r.instruct = (Instruct*) realloc(r.instruct, r.size);
			}
			// 读取opcode
			char opcode[13];
			copy_opcode(opcode);
			current_instruct->opcode = parse_opcode(opcode);
			// 读取参数
			read_parameters(current_instruct);
			// 换行
			skip('\n');
			// 填充完毕，指针移位
			r.size += sizeof(Instruct);
			current_instruct++;
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
	// ● 是否为某些字符中的一个？
	//-------------------------------------------------------------------------
	bool Assembler::check(char c, const char* s) {
		return strchr(s, (unsigned char) c) != NULL;
	}
	//-------------------------------------------------------------------------
	// ● 跳过某一字符
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
	// ● 跳过某些字符组成的串
	//-------------------------------------------------------------------------
	void Assembler::skip(const char* s, const char* error_msg) {
		size_t len = strspn(prg, s);
		if (!len) error(error_msg);
		prg += len;
	}
	//-------------------------------------------------------------------------
	// ● 复制opcode
	//-------------------------------------------------------------------------
	void Assembler::copy_opcode(char* buf) {
		for (size_t i = 0; i < 12; i++) {
			if (check(prg[i], " \t\v\n") {
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
	}
	//-------------------------------------------------------------------------
	// ● 读取立即数参数
	//-------------------------------------------------------------------------
	uint32_t Assembler::read_immediate_u32() {
		skip(" \t\v");
		if (!is_digit(*prg)) error("expected digit");
		long long n;
		if (prg[0] == '0' && prg[1] == 'x') {
			char* end;
			n = strtoll(prg + 2, &end, 16);
			prg = (const char*) end;
		} else {
			n = atoll(prg);
		}
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
		skip(" \t\v");
		skip('$');
		if (!is_digit(*prg)) error("expected digit");
		int reg = atoi(prg);
		if (reg < 0) error("register no. can't be negative");
		if ((size_t) reg > VM::REGISTER_COUNT) error("register no. too large");
		while (is_digit(*prg)) prg++;
		return reg;
	}
}
