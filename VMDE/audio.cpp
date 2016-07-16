//=============================================================================
// ■ audio.cpp
//-----------------------------------------------------------------------------
//   提供声音支持。
//=============================================================================

#include "global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 播放三角波
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	void play_triangle(int freq) {
	}
	//-------------------------------------------------------------------------
	// ● 播放三角波时内部使用的回调函数
	//-------------------------------------------------------------------------
	int play_triangle_callback(
		const void* input_buffer __attribute__((unused)),
		void* output_buffer,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo* time_info __attribute__((unused)),
		PaStreamCallbackFlags status_flags __attribute__((unused)),
		void* user_data
	) {
		float* output = (float*) output_buffer;
		struct triangle_data* data = (struct triangle_data*) user_data;
		while (frames_per_buffer > 0) {
			get_next_triangle_value(data);
			TWICE(
				*output = data->value;
				output++; frames_per_buffer--;
			)
		}
		return paContinue;
	}
	//-------------------------------------------------------------------------
	// ● 计算下一个值使输出呈三角波形
	//-------------------------------------------------------------------------
	void get_next_triangle_value(struct triangle_data* data) {
		data->value += data->delta;
		if (data->value > 1.0f) {
			data->value = 1.0f;
			data->delta = -data->delta;
		} else if (data->value < -1.0f) {
			data->value = -1.0f;
			data->delta = -data->delta;
		}
	}
}
