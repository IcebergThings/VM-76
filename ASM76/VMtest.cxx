//==============================================================================
// ■ VMtest 虚拟机测试程序
//==============================================================================

#include "ASM76.hpp"

using namespace ASM76;

Instruct test_prgm[] = {
	{LCMM,0x4000000,0},
	{DATI,0xABCD1234,51},
	{SLLA,0x2000000,50},
	{LDLA,0x2000000,71},
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
