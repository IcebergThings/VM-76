//=============================================================================
// ■ VError.hpp
//=============================================================================

#include <cstdio>

namespace V {
	inline void error(const char* x) {
		fprintf(stderr, "%s\n", x);
		perror("perror()");
		exit(9);
	}
}
