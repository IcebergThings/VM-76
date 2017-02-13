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
	{_HLT, 0, 0},
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
	{_HLT, 0, 0},
};

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init_environment();

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

	printf("===== TEST END =====\n");

	return 0;
}
