//=============================================================================
// ■ audio.hpp
//-----------------------------------------------------------------------------
//   处理播放声音的模块。
//=============================================================================

#ifndef _AUDIO_HPP
#define _AUDIO_HPP
#define pa_callback(name) \
	int name( \
		const void* input_buffer, \
		void* output_buffer, \
		unsigned long frame_count, \
		const PaStreamCallbackTimeInfo* time_info, \
		PaStreamCallbackFlags status_flags, \
		void* user_data \
	)
namespace Audio {
	//-------------------------------------------------------------------------
	// ● Channel
	//-------------------------------------------------------------------------
	class Channel {
		bool active = true;
	};
	class Channel_Mute : public Channel {
	};
	class Channel_Triangle : public Channel {
		float value;
		float delta;
		void play_triangle(float freq);
		void get_next_triangle_value(struct triangle_data* data);
	};
	class Channel_Sine : public Channel {
		float index;
		float index_delta;
		bool minus;
		float value; // for convenience only
		void play_sine(float freq);
		void get_next_sine_value(struct sine_data* data);
	};
	class Channel_Vorbis : public Channel {
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
	} vorbis;
	//-------------------------------------------------------------------------
	// ● 模块变量
	//-------------------------------------------------------------------------
	extern PaStream* stream;
	#define AUDIO_CHANNELS_SIZE ((size_t) 16)
	extern Channel* channels[AUDIO_CHANNELS_SIZE];
	//-------------------------------------------------------------------------
	// ● 模块级函数
	//-------------------------------------------------------------------------
	void init();
	void terminate();
	void ensure_no_error(PaError err);
	pa_callback(callback);
	void stop();
	void stop_waves();
	void compact_active_sounds_array();
	void play_sound(const char* filename, bool loop);
	void decode_vorbis(struct active_sound* sound);
	void decode_vorbis_thread(struct active_sound* sound);
}
#endif
