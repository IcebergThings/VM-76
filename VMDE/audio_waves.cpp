//=============================================================================
// ■ audio_waves.cpp
//-----------------------------------------------------------------------------
//   Audio命名空间中播放特定波形的函数组（分割定义）。
//=============================================================================

#include "global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 全局变量
	//   一些宏常量被定义在global.hpp中。
	//-------------------------------------------------------------------------
	PaStream* wave_stream;
	struct wave_callback_data wave_data;
	// 为了避免反复计算，将正弦值存储在这里。
	// 当AUDIO_SINE_TABLE_SIZE为256时，其分布为
	// [0] = sin 0
	// [64] = sin ⅛π
	// [128] = sin ¼π
	// [192] = sin ⅜π
	float sine_table[AUDIO_SINE_TABLE_SIZE];
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init_waves() {
		populate_sine_table();
		wave_data.type = 0;
		// 44100Hz
		wave_data.sample_rate = 44100.0;
		ensure_no_error(Pa_OpenDefaultStream(
			&wave_stream,
			// 无声输入 - 立体声输出、32位浮点数
			0, 2, paFloat32,
			wave_data.sample_rate,
			// 256格缓冲区
			256,
			play_wave_callback, &wave_data
		));
	}
	//-------------------------------------------------------------------------
	// ● 播放波形时使用的回调函数
	//-------------------------------------------------------------------------
	pa_callback(play_wave_callback) {
		float* output = (float*) output_buffer;
		struct wave_callback_data* data = (struct wave_callback_data*) user_data;
		// Magic. 吔屎啦PortAudio！
		#define FEED_AUDIO_DATA(value) do { \
			*output++ = (value); \
			*output++ = (value); \
			frame_count--; \
		} while (false)
		while (frame_count) switch (data->type) {
			case 0:
				FEED_AUDIO_DATA(.0f);
				break;
			case 1:
				get_next_triangle_value(&data->data.triangle);
				FEED_AUDIO_DATA(data->data.triangle.value);
				break;
			case 2:
			default:
				get_next_sine_value(&data->data.sine);
				FEED_AUDIO_DATA(data->data.sine.value);
				break;
			case 3:
				// Channels are different channels.
				// Values are different values.
				FEED_AUDIO_DATA((float) (
					(double) rand() / (double) RAND_MAX * 2.0 - 1.0
				));
				break;
		}
		#undef FEED_AUDIO_DATA
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
	// ● 向正弦表中填充数据
	//-------------------------------------------------------------------------
	void populate_sine_table() {
		float k = 0.5f / (float) AUDIO_SINE_TABLE_SIZE * Util::PIf;
		for (size_t i = 0; i < AUDIO_SINE_TABLE_SIZE; i++) sine_table[i] = sin(i * k);
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
