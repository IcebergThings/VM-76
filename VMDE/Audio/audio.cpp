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
	// 给个地方让各个声音通道撒野
	float channel_buffers[AUDIO_CHANNELS_SIZE][AUDIO_BUFFER_SIZE * 2];
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
	void play_sound(const char* filename, bool loop, float volume) {
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
		channels[slot]->volume = volume;
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
		// 寻找想要播放的声音通道
		size_t active_channels[AUDIO_CHANNELS_SIZE] = {0};
		size_t active_channels_count = 0;
		for (size_t i = 0; i < AUDIO_CHANNELS_SIZE; i++) {
			if (channels[i] && channels[i]->active) {
				// 让它们在channel_buffers里写入想要播放的内容
				channels[i]->fill(channel_buffers[i], frame_count);
				active_channels[active_channels_count] = i;
				active_channels_count++;
			}
		}
		// 本来是没有声音的，
		memset(output_buffer, 0, frame_count * sizeof(float) * 2);
		// 但是对于每个想要播放的声音通道，
		for (size_t i = 0; i < active_channels_count; i++) {
			float* buf = (float*) output_buffer;
			float* chbuf = channel_buffers[active_channels[i]];
			Channel* ch = channels[active_channels[i]];
			// 都要对每格缓冲区进行加和。
			for (size_t j = 0; j < frame_count; j++) {
				TWICE {
					*buf++ += Util::clamp((*chbuf++) * ch->volume, -1.0f, 1.0f);
				}
			}
		}
		return paContinue;
	}
}
