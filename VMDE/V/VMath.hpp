//=============================================================================
// ■ VMath.hpp
//-----------------------------------------------------------------------------
//   提供一些标准库里没有的数学函数。
//=============================================================================

namespace VMath {
	//-------------------------------------------------------------------------
	// ● 声明
	//-------------------------------------------------------------------------
	extern const double float PIf;
	extern const double PI;
	#define PId PI
	extern const long double PIl;
	void init();
	void populate_sine_table();
	#define VMATH_SINE_TABLE_SIZE ((size_t) 256)
	extern float sine_table[VMATH_SINE_TABLE_SIZE];
	//-------------------------------------------------------------------------
	// ● clamp
	//-------------------------------------------------------------------------
	template <class T> T clamp(T x, T min, T max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
}
