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
	const size_t vf_buffer_size = 4096;
	PaStream* wave_stream;
	struct callback_data data;
	struct active_sound* active_sounds[16] = {NULL};
	size_t active_sound_count = 0;
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
			&wave_stream,
			// 无声输入 - 立体声输出、32位浮点数
			0, 2, paFloat32,
			data.sample_rate,
			// 256格缓冲区
			256,
			play_callback, &data
		));
		ensure_no_error(Pa_StartStream(wave_stream));
	}
	//-------------------------------------------------------------------------
	// ● 释放
	//-------------------------------------------------------------------------
	void wobuzhidaozhegefangfayinggaijiaoshenmemingzi() {
		ensure_no_error(Pa_StopStream(wave_stream));
		ensure_no_error(Pa_CloseStream(wave_stream));
		Pa_Terminate();
	}
	//-------------------------------------------------------------------------
	// ● 统一处理错误
	//-------------------------------------------------------------------------
	void ensure_no_error(PaError err) {
		if (err >= 0) return;
		Pa_Terminate();
		rb_raise(
			rb_eRuntimeError,
			"PortAudio error %d: %s",
			err, Pa_GetErrorText(err)
		);
	}
	//-------------------------------------------------------------------------
	// ● 播放波形时使用的回调函数
	//-------------------------------------------------------------------------
	int play_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	) {
		float* output = (float*) output_buffer;
		struct callback_data* data = (struct callback_data*) user_data;
		// Magic. 吔屎啦PortAudio！
		#define FEED_AUDIO_DATA(value) do { \
			*output++ = (value); \
			*output++ = (value); \
			frame_count--; \
		} while (false)
		while (frame_count > 0) switch (data->type) {
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
				*((float*) 0) = .0f; // 音频就是爆炸！
				FEED_AUDIO_DATA(.0f);
				break;
			case 4:
				FEED_AUDIO_DATA((float) (
					(double) rand() / (double) RAND_MAX * 2.0d - 1.0d
				));
				break;
		}
		#undef FEED_AUDIO_DATA
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
	// ● 计算正弦函数的下一值
	//-------------------------------------------------------------------------
	void get_next_sine_value(struct sine_data* data) {
		data->index += data->index_delta;
		if (data->index > (float) sine_table_size) {
			data->index = sine_table_size * 2.0f - data->index;
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
	// ● 播放声音
	//-------------------------------------------------------------------------
	void play_sound(const char* filename) {
		compact_active_sounds_array();
		log("play sound %s", filename);
		struct active_sound* sound = new struct active_sound;
		sound->stream = NULL;
		// sound->file
		sound->file = fopen(filename, "rb");
		if (!sound->file) {
			delete sound;
			rb_raise(rb_eIOError, "can't open this file: %s", filename);
		}
		// sound->vf
		if (ov_open_callbacks(
			sound->file, &sound->vf,
			NULL, 0, OV_CALLBACKS_NOCLOSE
		) < 0) {
			delete sound;
			fclose(sound->file);
			rb_raise(rb_eRuntimeError, "can't open ogg vorbis file: %s", filename);
		}
		// sound->stream
		ensure_no_error(Pa_OpenDefaultStream(
			&sound->stream, 0, 2, paFloat32, 44100,
			256, play_sound_callback, sound
		));
		// sound->*_head
		sound->play_head = 0;
		sound->load_head = 0;
		// etc
		sound->eof = false;
		active_sounds[active_sound_count] = sound;
		active_sound_count++;
		decode_vorbis(sound);
		// sound->decode_thread
		sound->decode_thread = new thread(decode_vorbis_thread, sound);
		ensure_no_error(Pa_StartStream(sound->stream));
	}
	//-------------------------------------------------------------------------
	// ● 扔掉active_sounds中已经播放完的条目
	//-------------------------------------------------------------------------
	void compact_active_sounds_array() {
		size_t size = ARRAY_SIZE(active_sounds);
		for (size_t i = 0; i < size; i++) {
			if (!active_sounds[i]) continue;
			PaError active = Pa_IsStreamActive(active_sounds[i]->stream);
			ensure_no_error(active);
			if (!active) {
				Pa_CloseStream(active_sounds[i]->stream);
				ov_clear(&active_sounds[i]->vf);
				fclose(active_sounds[i]->file);
				active_sounds[i]->decode_thread->join();
				delete active_sounds[i]->decode_thread;
				delete active_sounds[i];
				active_sounds[i] = NULL;
			}
		}
	}
	//-------------------------------------------------------------------------
	// ● 播放声音的回调函数
	//-------------------------------------------------------------------------
	int play_sound_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	) {
		float* output = (float*) output_buffer;
		struct active_sound* sound = (struct active_sound*) user_data;
		while (frame_count > 0) {
			if (sound->play_head < sound->load_head) {
				size_t index = sound->play_head % vf_buffer_size;
				*output++ = sound->vf_buffer[0][index];
				*output++ = sound->vf_buffer[1][index];
				sound->play_head++;
			} else {
				TWICE *output++ = .0f;
			}
			frame_count--;
		}
		return sound->eof ? paComplete : paContinue;
	}
	//-------------------------------------------------------------------------
	// ● 解码文件来填active_sound结构中的缓冲区
	//-------------------------------------------------------------------------
	void decode_vorbis(struct active_sound* sound) {
		#ifdef DEBUG
			FILE* f = fopen("rubbish.raw", "wb");
		#endif
		size_t t;
		while ((t = sound->load_head - sound->play_head) < vf_buffer_size) {
			// After ov_read_float(), tmp_buffer will be changed.
			float** tmp_buffer;
			long ret = ov_read_float(
				&sound->vf,
				&tmp_buffer,
				vf_buffer_size - t,
				&sound->bitstream
			);
			if (ret > 0) {
				for (long i = 0; i < ret; i++) {
					size_t j = (sound->load_head + i) % vf_buffer_size;
					sound->vf_buffer[0][j] = tmp_buffer[0][i];
					sound->vf_buffer[1][j] = tmp_buffer[1][i];
					#ifdef DEBUG
						fprintf(f, "%ld %zu\r\n", i, j);
						fwrite(&tmp_buffer[0][i], sizeof(float), 1, f);
					#endif
				}
				sound->load_head += ret;
			} else if (ret == 0) {
				while (sound->load_head - sound->play_head < vf_buffer_size) {
					size_t j = sound->load_head % vf_buffer_size;
					sound->vf_buffer[0][j] = .0f;
					sound->vf_buffer[1][j] = .0f;
					sound->load_head++;
				}
				sound->eof = true;
				break;
			} else if (ret == OV_EBADLINK) {
				rb_raise(rb_eIOError, "bad vorbis data (OV_EBADLINK)");
			} else if (ret == OV_EINVAL) {
				rb_raise(rb_eIOError, "bad vorbis data (OV_EINVAL)");
			}
			// We must not free(tmp_buffer). It isn't ours.
		}
		#ifdef DEBUG
			fclose(f);
			exit(10);
		#endif
	}
	//-------------------------------------------------------------------------
	// ● 解码线程函数
	//-------------------------------------------------------------------------
	void decode_vorbis_thread(struct active_sound* sound) {
		while (!sound->eof) decode_vorbis(sound);
	}
}
