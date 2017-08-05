//=============================================================================
// ■ channel_sine.cpp
//-----------------------------------------------------------------------------
//   正弦波。
//=============================================================================

#include "../global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 构造
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	Channel_Sine::Channel_Sine(float freq) {
		index = .0f;
		index_delta = VMATH_SINE_TABLE_SIZE / ((float) AUDIO_SAMPLE_RATE / 4 / freq);
		minus = false;
	}
	//-------------------------------------------------------------------------
	// ● 填充缓冲区
	//-------------------------------------------------------------------------
	void Channel_Sine::fill(float* buf, unsigned long n) {
		while (n) {
			float v = next();
			FEED_AUDIO_DATA(buf, v);
			n--;
		}
	}
	//-------------------------------------------------------------------------
	// ● 计算正弦函数的下一值
	//-------------------------------------------------------------------------
	float Channel_Sine::next() {
		index += index_delta;
		if (index >= (float) VMATH_SINE_TABLE_SIZE) {
			index = VMATH_SINE_TABLE_SIZE * 2.0f - index;
			index_delta = -index_delta;
		} else if (index < 0) {
			index = -index;
			index_delta = -index_delta;
			minus = !minus;
		}
		float value = V::Math::sine_table[(size_t) (int) index];
		if (minus) value = -value;
		return value;
	}
}
