//=============================================================================
// ■ VMtest.cxx
//-----------------------------------------------------------------------------
//   虚拟机测试程序。
//   扩展名用了.cxx，意思是“×不要自动编译”。
//=============================================================================

#include "ASM76.hpp"

using namespace ASM76;

Instruct test_prgm[] = {
	{LCMM,0x4000000,0},
	{LDLA,0x400000,0},
	{LDLA,0x1000000,1},
	{LDIA,0x100000c,9},
	{LDBA,0x1000018,13},
	{_HLT, 0, 0},
};

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init_environment();
	VM* v = new VM(test_prgm, sizeof(test_prgm));

	v->execute();
	v->dump_registers();

	return 0;
}
