//=============================================================================
// ■ audio.hpp
//-----------------------------------------------------------------------------
//   处理播放声音的模块。
//=============================================================================

#ifndef _AUDIO_HPP
#define _AUDIO_HPP
namespace Audio {
	//-------------------------------------------------------------------------
	// ● 宏魔法
	//-------------------------------------------------------------------------
	// FEED_AUDIO_DATA - 给buf喂数据的快捷方法
	// 因为是立体声（两个声道），所以给*buf++赋值两次。
	// 注意：调用一次此宏，只需减少一次n！
	#define FEED_AUDIO_DATA(buf, value) do { \
		*buf++ = (value); \
		*buf++ = (value); \
	} while (false)
	//-------------------------------------------------------------------------
	// ● Channel
	//-------------------------------------------------------------------------
	class Channel {
	public:
		bool active = true;
		// 在Audio被PortAudio要求回调时，回调函数会调用此函数。
		virtual void fill(float* buf, unsigned long n) = 0;
	};
	class Channel_Mute : public Channel {
	};
	class Channel_Triangle : public Channel {
	private:
		float value;
		float delta;
		float next();
	public:
		Channel_Triangle(float freq);
	};
	class Channel_Sine : public Channel {
	private:
		float index;
		float index_delta;
		bool minus;
		float next();
	public:
		Channel_Sine(float freq);
	};
	class Channel_Vorbis : public Channel {
		FILE* f;
		OggVorbis_File vf;
		// AUDIO_VF_BUFFER_SIZE - 解码缓冲区的格数
		// 由于VRingBuffer的实现，实际可用的格数比此数值少1。
		#define AUDIO_VF_BUFFER_SIZE ((size_t) 4096)
		VRingBuffer<float, AUDIO_VF_BUFFER_SIZE> vf_buf;
		bool eof;
		bool loop;
		int bitstream;
		thread* decode_thread;
	public:
		void decode();
		static void decode_vorbis_thread(Channel_Vorbis* ch);
		Channel_Vorbis(const char* filename, bool loop);
		~Channel_Vorbis();
	};
	//-------------------------------------------------------------------------
	// ● 模块常量和变量
	//-------------------------------------------------------------------------
	// 采样率
	#define AUDIO_SAMPLE_RATE 48000
	// 缓冲区格数
	#define AUDIO_BUFFER_SIZE 256
	extern PaStream* stream;
	#define AUDIO_CHANNELS_SIZE ((size_t) 16)
	extern Channel* channels[AUDIO_CHANNELS_SIZE];
	//-------------------------------------------------------------------------
	// ● 模块级函数
	//-------------------------------------------------------------------------
	void init();
	void terminate();
	void ensure_no_error(PaError err);
	int callback(
		const void* input_buffer,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags status_flags,
		void* user_data
	);
	void stop();
	void stop_waves();
	void compact_channels();
	void play_sound(const char* filename, bool loop);
	void decode_vorbis(struct active_sound* sound);
	void decode_vorbis_thread(struct active_sound* sound);
}
#endif
