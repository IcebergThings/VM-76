//=============================================================================
// ■ channel_noise.cpp
//-----------------------------------------------------------------------------
//   这个声音通道输出白噪音。
//=============================================================================

#include <VMDE.hpp>

namespace Audio {
	void Channel_Noise::fill(float* buf, unsigned long n) {
		while (n) {
			// Channels are different channels.
			// Values are different values.
			FEED_AUDIO_DATA(buf, (float) (
				(double) rand() / (double) RAND_MAX * 2.0 - 1.0
			));
			n--;
		}
	}
}
