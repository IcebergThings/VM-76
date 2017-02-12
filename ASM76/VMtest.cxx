//==============================================================================
// ■ VMtest 虚拟机测试程序
//==============================================================================

#include "ASM76.hpp"

using namespace ASM76;

Instruct test_prgm[] = {
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

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init_environment();
	VM* v = new VM(test_prgm, sizeof(test_prgm));

	v->execute();
	v->dump_registers();

	return 0;
}
