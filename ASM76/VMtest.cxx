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
	{JI7A, 0x1000000 + 3 * sizeof(Instruct),0},
	{HALT, 0, 0},
};

const char* const asm_test =
	"\t \t \t \t # Comments.\n \n\t\n"
	"\tNOOP\n"
	//" DATB $1 2\n"
	//"DATI $2 0xAbCdEf00\n"
	//"DATI $6 1000\n"
	"HALT\n";

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init();

	printf("===== Memory =====\n");
	VM* v = new VM(mem_test_prgm, sizeof(mem_test_prgm));

	v->execute();
	v->dump_registers();
	delete v;

	printf("===== Basic Algebra =====\n");
	v = new VM(basic_algebra_test_prgm, sizeof(basic_algebra_test_prgm));

	v->execute();
	v->dump_registers();
	delete v;

	printf("===== Flow Control & Logistics =====\n");
	v = new VM(flow_control_test_prgm, sizeof(flow_control_test_prgm));

	v->execute();
	v->dump_registers();
	delete v;

	printf("===== Assembler =====\n");
	Assembler a(asm_test);
	Instruct* i = a.assemble();
	printf("%d\n", i[0].opcode);

	printf("===== TEST END =====\n");

	return 0;
}
