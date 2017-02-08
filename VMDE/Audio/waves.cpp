//=============================================================================
// ■ waves.cpp
//-----------------------------------------------------------------------------
//   Audio命名空间中播放特定波形的函数组（分割定义）。
//=============================================================================

#include "../global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 播放波形时使用的回调函数
	//-------------------------------------------------------------------------
	pa_callback(play_callback) {
		float* output = (float*) output_buffer;
		struct wave_callback_data* data = (struct wave_callback_data*) user_data;
		while (frame_count) switch (data->type) {
			case 1:
				get_next_triangle_value(&data->data.triangle);
				FEED_AUDIO_DATA(data->data.triangle.value);
				break;
			case 2:
			default:
				get_next_sine_value(&data->data.sine);
				FEED_AUDIO_DATA(data->data.sine.value);
				break;
		}
		return paContinue;
	}
	//-------------------------------------------------------------------------
	// ● 停止播放波形
	//-------------------------------------------------------------------------
	void stop_waves() {
		wave_data.type = 0;
	}
	//-------------------------------------------------------------------------
	// ● 播放三角波
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	void play_triangle(float freq) {
		wave_data.data.triangle.value = -1.0f;
		wave_data.data.triangle.delta =
			2.0f / ((float) wave_data.sample_rate / freq / 2);
		wave_data.type = 1;
	}
	//-------------------------------------------------------------------------
	// ● 计算下一个值使输出呈三角波形
	//-------------------------------------------------------------------------
	void get_next_triangle_value(struct triangle_data* data) {
		data->value += data->delta;
		if (data->value > 1.0f) {
			data->value = 2.0f - data->value;
			data->delta = -data->delta;
		} else if (data->value < -1.0f) {
			data->value = -2.0f - data->value;
			data->delta = -data->delta;
		}
	}
	//-------------------------------------------------------------------------
	// ● 播放正弦波
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	void play_sine(float freq) {
		wave_data.data.sine.index = .0f;
		wave_data.data.sine.index_delta =
			AUDIO_SINE_TABLE_SIZE / ((float) wave_data.sample_rate / 4 / freq);
		wave_data.data.sine.minus = false;
		wave_data.type = 2;
	}
	//-------------------------------------------------------------------------
	// ● 计算正弦函数的下一值
	//-------------------------------------------------------------------------
	void get_next_sine_value(struct sine_data* data) {
		data->index += data->index_delta;
		if (data->index >= (float) AUDIO_SINE_TABLE_SIZE) {
			data->index = AUDIO_SINE_TABLE_SIZE * 2.0f - data->index;
			data->index_delta = -data->index_delta;
		} else if (data->index < 0) {
			data->index = -data->index;
			data->index_delta = -data->index_delta;
			data->minus = !data->minus;
		}
		data->value = sine_table[(size_t) (int) data->index];
		if (data->minus) data->value = -data->value;
	}
	//-------------------------------------------------------------------------
	// ● I'd like to throw something here silently. Nobody'd find them.
	//   They'll eat up at least 10 bytes of no use. That's fine.
	//-------------------------------------------------------------------------
	long long ago; // in a galaxy far far away
	short life; // play more
}
