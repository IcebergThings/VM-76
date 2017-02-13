//=============================================================================
// ■ VExplosionTest.cxx
//-----------------------------------------------------------------------------
//   You need to manually compile this using any C++ compiler.
//=============================================================================

#include <cstdio>
#include "VExplosion.hpp"

int main() {
	printf(
		"Choose an implementation of EXPLOSION:\n"
		"\t[0] EXPLOSION\n"
		"\t[1] EXPLOSION_ASSIGNMENT\n"
		"\t[2] EXPLOSION_MEMORY\n"
		"\t[3] EXPLOSION_SIGNAL\n"
		"\t[4] EXPLOSION_CONSTSTR\n"
		"\t[5] EXPLOSION_ASM\n"
		"\t[6] EXPLOSION_ASSIGNMENT_SIMPLE\n"
		"\t[7] EXPLOSION_ASSIGNMENT_CALL\n"
		"> "
	);
	switch (fgetc(stdin)) {
	case '0':
		EXPLOSION;
		break;
	case '1':
		EXPLOSION_ASSIGNMENT;
		break;
	case '2':
		EXPLOSION_MEMORY;
		break;
	case '3':
		EXPLOSION_SIGNAL;
		break;
	case '4':
		EXPLOSION_CONSTSTR;
		break;
	case '5':
		EXPLOSION_ASM;
		break;
	case '6':
		EXPLOSION_ASSIGNMENT_SIMPLE;
		break;
	case '7':
		EXPLOSION_CALL;
		break;
	default:
		puts("You have input an invalid character. Good bye!");
	}
}
