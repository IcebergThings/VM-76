//=============================================================================
// ■ audio.cpp
//-----------------------------------------------------------------------------
//   提供声音支持。
//=============================================================================

#include "global.hpp"

namespace Audio {
	PaStream* stream;
	struct triangle_data data;
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		ensure_no_error(Pa_Initialize());
		ensure_no_error(Pa_OpenDefaultStream(
			&stream,
			// 无声输入 - 立体声输出、32位浮点数、44100Hz
			0, 2, paFloat32, 44100,
			// 256格缓冲区
			256,
			play_triangle_callback, &data
		));
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
	// ● 播放三角波
	//    freq : 频率（Hz）
	//-------------------------------------------------------------------------
	void play_triangle(float freq) {
		data.value = -1.0f;
		data.delta = 2.0f / (44100.0f / freq / 2);
		ensure_no_error(Pa_StartStream(stream));
	}
	//-------------------------------------------------------------------------
	// ● 播放三角波时内部使用的回调函数
	//-------------------------------------------------------------------------
	int play_triangle_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	) {
		float* output = (float*) output_buffer;
		struct triangle_data* data = (struct triangle_data*) user_data;
		while (frames_per_buffer > 0) {
			get_next_triangle_value(data);
			TWICE {
				*output = data->value;
				output++; frames_per_buffer--;
			}
		}
		return paContinue;
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
}
