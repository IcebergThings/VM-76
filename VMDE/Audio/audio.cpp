//=============================================================================
// ■ audio.cpp
//-----------------------------------------------------------------------------
//   提供声音支持。
//=============================================================================

#include "../global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 全局变量
	//-------------------------------------------------------------------------
	// 唯一的输出流
	PaStream* stream;
	// 正在播放的声音通道列表
	Channel* channels[AUDIO_CHANNELS_SIZE] = {NULL};
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		ensure_no_error(Pa_Initialize());
		ensure_no_error(Pa_OpenDefaultStream(
			&stream,
			// 无声输入 - 立体声输出、32位浮点数
			0, 2, paFloat32,
			AUDIO_SAMPLE_RATE,
			AUDIO_BUFFER_SIZE,
			callback,
			NULL
		));
		ensure_no_error(Pa_StartStream(stream));
	}
	//-------------------------------------------------------------------------
	// ● 结束处理
	//-------------------------------------------------------------------------
	void terminate() {
		ensure_no_error(Pa_StopStream(stream));
		ensure_no_error(Pa_CloseStream(stream));
		Pa_Terminate();
	}
	//-------------------------------------------------------------------------
	// ● 统一处理错误
	//-------------------------------------------------------------------------
	void ensure_no_error(PaError err) {
		if (err >= 0) return;
		Pa_Terminate();
		log("PortAudio error %d: %s", err, Pa_GetErrorText(err));
	}
	//-------------------------------------------------------------------------
	// ● 停止播放
	//-------------------------------------------------------------------------
	void stop() {
		// TODO
		compact_channels();
	}
	//-------------------------------------------------------------------------
	// ● 寻找channels中的空位
	//   找不到空位时，返回-1。
	//-------------------------------------------------------------------------
	int free_slot() {
		compact_channels();
		for (size_t i = 0; i < AUDIO_CHANNELS_SIZE; i++) {
			if (!channels[i]) return i;
		}
		log("warning: no free slots in Audio::channels");
		return -1;
	}
	//-------------------------------------------------------------------------
	// ● 播放声音
	//   其实这个函数应该叫做draw_sound，不然都对不起Draw Engine这个名字。
	//-------------------------------------------------------------------------
	void play_sound(const char* filename, bool loop) {
		// Find a blank first.
		int slot = free_slot();
		log("play sound %s[%d]", filename, slot);
		if (slot < 0) {
			log(
				"unable to play sound %s"
				" because of my stingy programmer that only gave me %zu slots",
				filename, AUDIO_CHANNELS_SIZE
			);
			return;
		}
		// Fill in the blanks with the words you have heard.
		channels[slot] = new Channel_Vorbis(filename, loop);
	}
	//-------------------------------------------------------------------------
	// ● 扔掉active_sounds中已经播放完的条目
	//-------------------------------------------------------------------------
	void compact_channels() {
		for (size_t i = 0; i < AUDIO_CHANNELS_SIZE; i++) {
			if (!channels[i]) continue;
			if (!channels[i]->active) {
				delete channels[i];
				channels[i] = NULL;
			}
		}
	}
	//-------------------------------------------------------------------------
	// ● 播放声音的回调函数
	//-------------------------------------------------------------------------
	int callback(
		const void* input_buffer,
		void* output_buffer,
		unsigned long frame_count,
		const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags status_flags,
		void* user_data
	) {
		float* buf = (float*) output_buffer;
		if (channels[0]) {
			channels[0]->fill(buf, frame_count);
		} else {
			memset(output_buffer, 0, frame_count * sizeof(float));
		}
		return paContinue;
	}
}
