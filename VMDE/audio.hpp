//=============================================================================
// ■ audio.hpp
//-----------------------------------------------------------------------------
//   audio.cpp & audio_waves.cpp 的头文件
//=============================================================================

#include "global.hpp"

#ifndef _INCLUDE_AUDIO_H
#define _INCLUDE_AUDIO_H

namespace Audio {
	struct triangle_data {
		float value;
		float delta;
	};
	struct sine_data {
		float index;
		float index_delta;
		bool minus;
		float value; // for convenience only
	};
	struct wave_callback_data {
		double sample_rate;
		// type = 0……静音；1……三角波；2……正弦波；3……白噪音
		// 为啥不用枚举？因为太麻烦了！
		int type;
		union {
			struct triangle_data triangle;
			struct sine_data sine;
		} data;
	};
	struct active_sound {
		PaStream* stream;
		FILE* file;
		OggVorbis_File vf;
		#define AUDIO_VF_BUFFER_SIZE ((size_t) 4096)
		float vf_buffer[2][AUDIO_VF_BUFFER_SIZE];
		size_t play_head;
		size_t load_head;
		bool eof;
		bool loop;
		int bitstream;
		thread* decode_thread;
	};
	extern PaStream* wave_stream;
	extern struct wave_callback_data wave_data;
	#define AUDIO_ACTIVE_SOUND_SIZE ((size_t) 16)
	extern struct active_sound* active_sounds[AUDIO_ACTIVE_SOUND_SIZE];
	#define AUDIO_SINE_TABLE_SIZE ((size_t) 256)
	extern float sine_table[AUDIO_SINE_TABLE_SIZE];
	void init();
	void init_waves();
	void wobuzhidaozhegefangfayinggaijiaoshenmemingzi();
	void ensure_no_error(PaError err);
	int play_wave_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frames_per_buffer,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	);
	void stop();
	void stop_waves();
	void play_triangle(float freq);
	void get_next_triangle_value(struct triangle_data* data);
	void play_sine(float freq);
	void populate_sine_table();
	void get_next_sine_value(struct sine_data* data);
	void compact_active_sounds_array();
	void play_sound(const char* filename, bool loop);
	int play_sound_callback(
		const void* input_buffer UNUSED,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info UNUSED,
		PaStreamCallbackFlags status_flags UNUSED,
		void* user_data
	);
	void decode_vorbis(struct active_sound* sound);
	void decode_vorbis_thread(struct active_sound* sound);
}

#endif
