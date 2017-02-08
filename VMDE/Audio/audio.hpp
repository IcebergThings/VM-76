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
		float volume = 1.0f;
		// 在Audio被PortAudio要求回调时，回调函数会调用此函数。
		virtual void fill(float* buf, unsigned long n) = 0;
		virtual ~Channel();
	};
	class Channel_Mute : public Channel {
		void fill(float* buf, unsigned long n);
	};
	class Channel_Noise : public Channel {
		void fill(float* buf, unsigned long n);
	};
	class Channel_Triangle : public Channel {
	private:
		float value;
		float delta;
		float next();
	public:
		Channel_Triangle(float freq);
		void fill(float* buf, unsigned long n);
	};
	class Channel_Sine : public Channel {
	private:
		float index;
		float index_delta;
		bool minus;
		float next();
	public:
		Channel_Sine(float freq);
		void fill(float* buf, unsigned long n);
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
		thread* thrd;
	public:
		void decode();
		static void decode_thread(Channel_Vorbis* ch);
		Channel_Vorbis(const char* filename, bool loop);
		~Channel_Vorbis();
		void fill(float* buf, unsigned long n);
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
	extern float channel_buffers[AUDIO_CHANNELS_SIZE][AUDIO_BUFFER_SIZE * 2];
	//-------------------------------------------------------------------------
	// ● 模块级函数
	//-------------------------------------------------------------------------
	void init();
	void terminate();
	void ensure_no_error(PaError err);
	void stop();
	int free_slot();
	void play_sound(const char* filename, bool loop, float volume = 1.0f);
	void compact_channels();
	int callback(
		const void* input_buffer,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags status_flags,
		void* user_data
	);
}
#endif
