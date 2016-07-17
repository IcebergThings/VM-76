//=============================================================================
// ■ audio.cpp
//-----------------------------------------------------------------------------
//   提供声音支持。
//=============================================================================

#include "global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 全局变量
	//-------------------------------------------------------------------------
	PaStream* stream;
	struct callback_data data;
	// 为了避免反复计算，将正弦值存储在这里。其分布为
	// [0] = sin 0
	// [64] = sin ⅛π
	// [128] = sin ¼π
	// [192] = sin ⅜π
	float sine_table[256];
	const size_t sine_table_size = ARRAY_SIZE(sine_table);
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		ensure_no_error(Pa_Initialize());
		populate_sine_table();
		data.type = 0;
		// 44100Hz
		data.sample_rate = 44100.0d;
		ensure_no_error(Pa_OpenDefaultStream(
			&stream,
			// 无声输入 - 立体声输出、32位浮点数
			0, 2, paFloat32,
			data.sample_rate,
			// 256格缓冲区
			256,
			play_callback, &data
		));
		ensure_no_error(Pa_StartStream(stream));
	}
	//-------------------------------------------------------------------------
	// ● 释放
	//-------------------------------------------------------------------------
	void wobuzhidaozhegefangfayinggaijiaoshenmemingzi() {
		ensure_no_error(Pa_StopStream(stream));
		ensure_no_error(Pa_CloseStream(stream));
		Pa_Terminate();
	}
	//-------------------------------------------------------------------------
	// ● 统一处理错误
	//-------------------------------------------------------------------------
	void ensure_no_error(PaError err) {
		if (err == paNoError) return;
		Pa_Terminate();
		rb_raise(
			rb_eRuntimeError,
			"PortAudio error %d: %s",
			err, Pa_GetErrorText(err)
		);
	}
	//-------------------------------------------------------------------------
	// ● 内部使用的回调函数
	//-------------------------------------------------------------------------
	int play_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	) {
		float* output = (float*) output_buffer;
		float* output_end = output + frames_per_buffer;
		struct callback_data* data = (struct callback_data*) user_data;
		while (output < output_end) {
			switch (data->type) {
				case 0:
					*output = .0f;
					output++;
					break;
				case 1:
					get_next_triangle_value(&data->data.triangle);
					TWICE {
						*output = data->data.triangle.value;
						output++;
					}
					break;
				case 2:
				default:
					{
						get_next_sine_value(&data->data.sine);
						float value = sine_table[(size_t) (int) data->data.sine.index];
						if (data->data.sine.minus) value = -value;
						TWICE {
							*output = value;
							output++;
						}
						break;
					}
				case 3:
					*((float*) 0) = .0f; // 音频就是爆炸！
					break;
			}
		}
		return paContinue;
	}
	//-------------------------------------------------------------------------
	// ● 停止播放
	//-------------------------------------------------------------------------
	void stop() {
		data.type = 0;
	}
	//-------------------------------------------------------------------------
	// ● 播放三角波
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	void play_triangle(float freq) {
		data.data.triangle.value = -1.0f;
		data.data.triangle.delta = 2.0f / ((float) data.sample_rate / freq / 2);
		data.type = 1;
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
		data.data.sine.index = .0f;
		data.data.sine.index_delta =
			sine_table_size / ((float) data.sample_rate / 4 / freq);
		data.data.sine.minus = false;
		data.type = 2;
	}
	//-------------------------------------------------------------------------
	// ● 向正弦表中填充数据
	//-------------------------------------------------------------------------
	void populate_sine_table() {
		float k = 0.5f / (float) sine_table_size * Util::PIf;
		for (size_t i = 0; i < sine_table_size; i++) sine_table[i] = sin(i * k);
	}
	//-------------------------------------------------------------------------
	// ● 计算正弦函数的下一自变量
	//-------------------------------------------------------------------------
	void get_next_sine_value(struct sine_data* data) {
		data->index += data->index_delta;
		if (data->index > (float) sine_table_size) {
			data->index_delta = -data->index_delta;
		} else if (data->index < 0) {
			data->index_delta = -data->index_delta;
			data->minus = !data->minus;
		}
	}
}
