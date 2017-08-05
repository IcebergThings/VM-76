//=============================================================================
// ■ VMtest.cxx
//-----------------------------------------------------------------------------
//   虚拟机测试程序。
//   扩展名用了.cxx，意思是“×不要自动编译”。
//=============================================================================

#include <ratio>
#include <chrono>
using namespace std;

#include "ASM76.hpp"
using namespace ASM76;

Instruct test_mem_i[] = {
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
Program test_mem_p {
	.size = sizeof(test_mem_i),
	.instruct = test_mem_i
};

Instruct test_basic_algebra_i[] = {
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
Program test_basic_algebra_p {
	.size = sizeof(test_basic_algebra_i),
	.instruct = test_basic_algebra_i
};

Instruct test_flow_control_i[] = {
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
Program test_flow_control_p {
	.size = sizeof(test_flow_control_i),
	.instruct = test_flow_control_i
};

const char* const test_asm =
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
	"DD 0x7676 0xEFEF 0xABAB 0xCDCD 0x0000\n"
	"# EOF\n";

const char* const test_bios =
	"DATI	0x76 $0\n"
	"SLIA	0x100 $0\n"
	"INTX	0x1 0x100\n"
	"DATI	0xAB $0\n"
	"SLIA	0x100 $0\n"
	"INTX	0x1 0x100\n"
	"HALT\n"
	"# EOF\n";

static uint32_t test_bios_call(uint8_t* input) {
	printf("TEST BIOS CALL: %x\n", *((uint32_t*) input));

	return 0x89ABCDEF;
}

Instruct test_speed_i[] = {
	{DATI, 0x1, 20},
	{DATI, 0x3000000, 60},
	{DATI, 0x2, 15},
	{ADDL, 3, 20},
	{CMPI, 3, 60},
	{JILA, 3 * sizeof(Instruct), 0},
	{HALT, 0, 0},
};
Program test_speed_p {
	.size = sizeof(test_speed_i),
	.instruct = test_speed_i
};

const char* const test_tag =
	"JMPA [end]\n"
	"ADDI $50 $100\n"
	"MINI $25 $50\n"
	"[end]\n"
	"HALT\n"
	"# EOF\n";

int main() {
	puts("+---------------------+");
	puts("| ASM 76 Test Program |");
	puts("+---------------------+");
	init();

	{
		puts("===== Memory =====");
		VM v(test_mem_p);
		v.execute(true);
		v.dump_registers();
	}

	{
		puts("===== Basic Algebra =====");
		VM v(test_basic_algebra_p);
		v.execute(true);
		v.dump_registers();
	}

	{
		puts("===== Disassembler =====");
		Disassembler d(test_flow_control_p);
		char* s = d.disassemble();
		puts(s);
		free(s);
	}

	{
		puts("===== Assembler =====");
		Assembler a(test_asm);
		a.scan();
		Program p = a.assemble();
		// Test assembler DD function
		printf("Data DD: %04x%08x%08x\n", p.instruct[9].opcode, p.instruct[9].a, p.instruct[9].b);
		// Test disassembler
		Disassembler d(p);
		char* s = d.disassemble();
		puts(s);
		free(s);

		puts("===== Flow Control & Logistics =====");
		VM v(p);
		v.execute(false);
		v.dump_registers();
		free(p.instruct);
	}

	{
		puts("===== Assembler: Tag Test =====");
		Assembler a(test_tag);
		a.scan();
		Program p = a.assemble();
		Disassembler d(p);
		char* s = d.disassemble();
		puts(s);
		free(s);
	}

	{
		puts("===== BIOS Test =====");
		BIOS* b = new BIOS(5);
		b->function_table[1] = &test_bios_call;

		Assembler a(test_bios);
		a.scan();
		Program p = a.assemble();

		VM v(p);
		v.firmware = b;
		v.execute(true);
		v.dump_registers();
	}

	{
		puts("===== Object Code =====");
		ObjectCode::write_file("test.obj", test_mem_p);
		Program p = ObjectCode::read_file("test.obj");
		Disassembler d(p);
		char* s = d.disassemble();
		puts(s);
		free(s);
		free(p.instruct);
	}

	{
		puts("===== Speed Test: 0x3000000 cycles =====");
		VM v(test_speed_p);

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

	puts("\n===== TEST END =====");

	return 0;
}
