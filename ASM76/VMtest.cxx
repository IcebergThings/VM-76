//=============================================================================
// ■ VMtest.cxx
//-----------------------------------------------------------------------------
//   虚拟机、汇编器、反汇编器、目标代码的测试程序。
//   扩展名用了.cxx，意思是“× 不要自动编译”。
//=============================================================================

#include <ratio>
#include <chrono>

#include "ASM76.hpp"
using namespace ASM76;

void test_mem() {
	puts("===== Memory =====");
	Instruct instruct[] = {
		{LCMM, 0x4000000, 0},
		{DATI, 0xABCD1234, 1},
		{DATB, 'V', 5},
		{DATB, 'M', 6},
		{DATB, '7', 7},
		{DATB, '6', 8},
		{SLLA, 0x2000000, 1},
		{SLIA, 0x2500000, 2},
		{SLBA, 0x2700000, 3},
		{LDLA, 0x2500000, 51},
		{LDIA, 0x2500000, 51},
		{LDBA, 0x2500000, 61},
		{HALT, 0, 0},
	};
	Program p {
		.size = sizeof(instruct),
		.instruct = instruct
	};
	VM v(p);
	v.execute(true);
	v.dump_registers();
}

void test_basic_algebra() {
	puts("===== Basic Algebra =====");
	Instruct instruct[] = {
		{LCMM, 0x100, 0},
		{DATI, 0x1, 1},
		// $1 = $1 + $1
		// 1, 2, 4, 8, 16, ...
		{ADDL, 1, 1},
		{ADDL, 1, 1},
		{ADDL, 1, 1},
		{ADDL, 1, 1},
		// Put the result somewhere else
		{MVRL, 1, 31},
		// Then divide it by 4
		{DATI, 0x4, 11},
		{DIVL, 1, 11},
		{HALT, 0, 0},
	};
	Program p {
		.size = sizeof(instruct),
		.instruct = instruct
	};
	VM v(p);
	v.execute(true);
	v.dump_registers();
}

void test_disassembler_assembler() {
	puts("===== Assembler =====");
	const char* const s1 =
		"# Comments.\n"
		"DATB	0x1 $20\n"
		"DATB	0xA $10\n"
		"DATB	0x2 $15\n"
		"[loop_start]\n"
		"ADDL	$15 $15\n"
		"ADDL	$3 $20\n"
		"CMPI	$3 $10\n"
		"PUSH	$15 1\n"
		"JILA	[loop_start]\n"
		"HALT\n"
		"RAWD	0x7676 0xEFEF 0xABAB 0xCDCD 0x0000\n"
		"# EOF\n";
	Assembler a(s1);
	a.scan();
	Program p = a.assemble();
	// Test assembler RAWD function
	printf(
		"Data RAWD: %04x%08x%08x\n",
		p.instruct[9].opcode,
		p.instruct[9].a,
		p.instruct[9].b
	);
	// Test disassembler
	Disassembler d(p);
	char* s2 = d.disassemble();
	puts(s2);
	free(s2);
}

void test_tag() {
	puts("===== Assembler: Tag Test =====");
	const char* const s1 =
		"JMPA [end]\n"
		"ADDI $50 $100\n"
		"MINI $25 $50\n"
		"[end]\n"
		"HALT\n"
		"# EOF\n";
	Assembler a(s1);
	a.scan();
	Program p = a.assemble();
	Disassembler d(p);
	char* s2 = d.disassemble();
	puts(s2);
	free(s2);
}

void test_flow_control() {
	puts("===== Flow Control & Logistics =====");
	Instruct instruct[] = {
		{DATB, 0x1, 20},
		{DATB, 0xA, 10},
		{DATB, 0x2, 15},
		{ADDL, 15, 15},
		{ADDL, 3, 20},
		{CMPI, 3, 10},
		{PUSH, 15, 1},
		{JILA, 3 * sizeof(Instruct), 0},
		{HALT, 0, 0},
	};
	Program p {
		.size = sizeof(instruct),
		.instruct = instruct
	};
	VM v(p);
	v.execute(false);
	v.dump_registers();
}

static uint32_t test_bios_call(uint8_t* input) {
	printf("TEST BIOS CALL: %x\n", *((uint32_t*) input));
	return 0x89ABCDEF;
}

void test_bios() {
	puts("===== BIOS Test =====");
	const char* const s =
		"DATI	0x76 $0\n"
		"SLIA	0x100 $0\n"
		"INTX	0x1 0x100\n"
		"DATI	0xAB $0\n"
		"SLIA	0x100 $0\n"
		"INTX	0x1 0x100\n"
		"HALT\n"
		"# EOF\n";

	BIOS b(5);
	b.function_table[1] = &test_bios_call;

	Assembler a(s);
	a.scan();
	Program p = a.assemble();

	VM v(p);
	v.firmware = &b;
	v.execute(true);
	v.dump_registers();
}

void test_object_code() {
	puts("===== Object Code =====");
	Instruct instruct[] = {
		{ADDL, 7, 6},
		{NOOP, 0, 0},
		{HALT, 0, 0},
	};
	Program p1 {
		.size = sizeof(instruct),
		.instruct = instruct
	};
	ObjectCode::write_file("test.obj", p1);
	Program p2 = ObjectCode::read_file("test.obj");
	Disassembler d(p2);
	char* s = d.disassemble();
	puts(s);
	free(s);
	free(p2.instruct);
}

void test_speed() {
	puts("===== Speed Test: 0x3000000 cycles =====");
	Instruct instruct[] = {
		{DATI, 0x1, 20},
		{DATI, 0x3000000, 60},
		{DATI, 0x2, 15},
		{ADDL, 3, 20},
		{CMPI, 3, 60},
		{JILA, 3 * sizeof(Instruct), 0},
		{HALT, 0, 0},
	};
	Program p {
		.size = sizeof(instruct),
		.instruct = instruct
	};
	VM v(p);

	// The type is chrono::time_point<chrono::high_resolution_clock>
	// and that is why people used auto.
	auto t1 = chrono::high_resolution_clock::now();
	v.execute(false);
	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double, milli> delay = t2 - t1;
	printf("Elapsed time: %lfms\nMIPS: %lf\n",
		delay.count(),
		((double) 0x3000000) * 3.0 / ((double) delay.count()) / 10000.0
	);
}

int main() {
	puts("+---------------------+");
	puts("| ASM 76 Test Program |");
	puts("+---------------------+");
	init();

	test_mem();
	test_basic_algebra();
	test_disassembler_assembler();
	test_tag();
	test_flow_control();
	test_bios();
	test_object_code();
	test_speed();

	putchar('\n');
	VMterminate();
	puts("# TEST END #");
}
