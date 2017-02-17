//=============================================================================
// ■ VMtest.cxx
//-----------------------------------------------------------------------------
//   虚拟机测试程序。
//   扩展名用了.cxx，意思是“×不要自动编译”。
//=============================================================================

#include "ASM76.hpp"

using namespace ASM76;

Instruct mem_test_prgm[] = {
	{LCMM,0x4000000,0},
	{DATI,0xABCD1234,1},
	{DATB,'V',5},
	{DATB,'M',6},
	{DATB,'7',7},
	{DATB,'6',8},
	{SLLA,0x2000000,1},
	{SLIA,0x2500000,2},
	{SLBA,0x2700000,3},
	{LDLA,0x2500000,51},
	{LDIA,0x2500000,51},
	{LDBA,0x2500000,61},
	{HALT, 0, 0},
};

Instruct basic_algebra_test_prgm[] = {
	{LCMM,0x100,0},
	{DATI,0x1,1},
	// $1 = $1 + $1
	// 1, 2, 4, 8, 16, ...
	{ADDL,1,1},
	{ADDL,1,1},
	{ADDL,1,1},
	{ADDL,1,1},
	// Put the result else where
	{MVRL,1,31},
	// Then divide 4
	{DATI,0x4,11},
	{DIVL,1,11},
	{HALT, 0, 0},
};

Instruct flow_control_test_prgm[] = {
	{DATB,0x1,20},
	{DATB,0xA,10},
	{DATB,0x2,15},
	{ADDL,15,15},
	{ADDL,3,20},
	{CMPI,3,10},
	{PUSH,15,1},
	{JILA, 0x1000000 + 3 * sizeof(Instruct),0},
	{HALT, 0, 0},
};

const char* const asm_test =
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
	"# EOF\n";

Instruct speed_test_prgm[] = {
	{DATI,0x1,20},
	{DATI,0x1000000,60},
	{DATI,0x2,15},
	{ADDL,3,20},
	{CMPI,3,60},
	{JILA, 0x1000000 + 3 * sizeof(Instruct),0},
	{HALT, 0, 0},
};

const char* const tag_test =
	"JMPA [end]\n"
	"ADDI $50 $100\n"
	"MINI $25 $50\n"
	"[end]\n"
	"HALT\n"
	"# EOF\n";

#include <ratio>
#include <chrono>
using namespace std;

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init();

	{
		printf("===== Memory =====\n");
		VM v({mem_test_prgm, sizeof(mem_test_prgm)});
		v.execute(true);
		v.dump_registers();
	}

	{
		printf("===== Basic Algebra =====\n");
		VM v({basic_algebra_test_prgm, sizeof(basic_algebra_test_prgm)});
		v.execute(true);
		v.dump_registers();
	}

	{
		printf("===== Disassembler =====\n");
		Disassembler d({flow_control_test_prgm, sizeof(flow_control_test_prgm)});
		char* s = d.disassemble();
		puts(s);
		free(s);
	}

	{
		printf("===== Assembler =====\n");
		Assembler a(asm_test);
		a.scan();
		Program p = a.assemble();
		Disassembler d(p);
		char* s = d.disassemble();
		puts(s);
		free(s);

		printf("===== Flow Control & Logistics =====\n");
		VM v(p);
		v.execute(false);
		v.dump_registers();
		free(p.instruct);
	}

	{
		printf("===== Assembler: Tag Test =====\n");
		Assembler a(tag_test);
		a.scan();
		Program p = a.assemble();
		Disassembler d(p);
		char* s = d.disassemble();
		puts(s);
		free(s);
	}

	{
		printf("===== Speed Test: 0x1000000 cycles =====\n");
		VM v({speed_test_prgm, sizeof(flow_control_test_prgm)});

		// The type is chrono::time_point<chrono::high_resolution_clock>
		// and that is why people used auto.
		auto t1 = chrono::high_resolution_clock::now();
		v.execute(false);
		auto t2 = chrono::high_resolution_clock::now();
		chrono::duration<double, milli> delay = t2 - t1;
		printf("Elapsed time: %fms\nMIPS: %f\n",
			delay.count(),
			0x1000000 * 3.0 / delay.count() / 10000.0
		);
	}

	printf("===== TEST END =====\n");

	return 0;
}
