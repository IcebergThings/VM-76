//=============================================================================
// ■ ObjectCode.cpp
//-----------------------------------------------------------------------------
//   处理目标代码。这里的目标代码格式如下：
//     魔数文件头    — A3 EF A3 E2 56 4D 37 36
//     指令大小      — 1E 00 00 00                   — 30字节
//     10字节/个指令 — 01 00 07 00 00 00 06 00 00 00 — #1 ADDL $7 $6
//                   — 00 00 00 00 00 00 00 00 00 00 — #2 NOOP
//                   — 28 00 00 00 00 00 00 00 00 00 — #3 HALT
//     任意尾部数据  — 01 23 45 67 89 AB CD EF
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 { namespace ObjectCode {
	const uint64_t magic = 0x36374d56e2a3efa3; // “ｏｂVM76” in GB2312
	Program read_file(const char* filename) {
		Program r;
		V::BinaryFileReader f(filename);
		if (f.read_u64() != magic) {
			printf("Error: magic is not correct\n");
			r.size = 0;
			r.instruct = NULL;
			return r;
		}
		r.size = f.read_u32();
		r.instruct = (Instruct*) malloc(r.size);
		for (size_t i = 0; i < r.size / sizeof(Instruct); i++) {
			r.instruct[i].opcode = f.read_u16();
			r.instruct[i].a = f.read_u32();
			r.instruct[i].b = f.read_u32();
		}
		return r;
	}
	bool write_file(const char* filename, Program program) {
		V::BinaryFileWriter f(filename);
		f.write_u64(magic);
		f.write_u32(program.size);
		for (size_t i = 0; i < program.size / sizeof(Instruct); i++) {
			f.write_u16(program.instruct[i].opcode);
			f.write_u32(program.instruct[i].a);
			f.write_u32(program.instruct[i].b);
		}
		return true;
	}
}}
