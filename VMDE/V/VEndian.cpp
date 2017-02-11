//=============================================================================
// ■ VEndian.cpp
//-----------------------------------------------------------------------------
//   判断尾端的函数组。据说这样的函数会被直接优化掉？
//=============================================================================

#include "../global.hpp"

namespace V {
	//-------------------------------------------------------------------------
	// ● 是否是小尾端？
	//-------------------------------------------------------------------------
	bool is_little_endian() {
		// 0x76或将成为VM/76项目中的0x55aa。
		// 或者说，是0x01020304更合适？
		union {
			uint32_t u32;
			uint8_t u8[4];
		} test = {0x76};
		return test.u8[0] == 0x76;
	}
}
