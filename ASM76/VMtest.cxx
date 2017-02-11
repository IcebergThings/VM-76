//==============================================================================
// ■ VMtest 虚拟机测试程序
//==============================================================================

#include "ASM76.hpp"

using namespace ASM76;

Instruct test_prgm[4] = {
	{LCMM,0x4000000,0},
	{0,0,0},
	{0,0,0},
	{_HLT, 0, 0},
};

int main() {

	printf("===== ASM 76 Test Program =====\n");

	init_environment();
	VM* v = new VM(test_prgm, sizeof(test_prgm));

	v->execute();

	return 0;
}
