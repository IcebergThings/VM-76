//=============================================================================
// ■ audio.cpp
//-----------------------------------------------------------------------------
//   提供声音支持。
//=============================================================================

#include "audio.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 全局变量
	//-------------------------------------------------------------------------
	// 唯一的输出流
	PaStream* stream;
	// 活动（正在播放中）的声音列表
	struct channel channels[AUDIO_CHANNELS_SIZE] = {NULL};
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		ensure_no_error(Pa_Initialize());
		ensure_no_error(Pa_OpenDefaultStream(
			&stream,
			// 无声输入 - 立体声输出、32位浮点数
			0, 2, paFloat32,
			// 48000Hz
			48000.0,
			// 256格缓冲区
			256,
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
		stop_waves();
		compact_active_sounds_array();
	}
	//-------------------------------------------------------------------------
	// ● 播放声音
	//   其实这个函数应该叫做draw_sound，不然都对不起Draw Engine这个名字。
	//-------------------------------------------------------------------------
	void play_sound(const char* filename, bool loop) {
		compact_active_sounds_array();
		log("play sound %s", filename);
		struct active_sound* sound = new struct active_sound;
		sound->stream = NULL;
		// Find a blank first.
		size_t first_free_slot = (size_t) -1;
		{
			bool found_a_blank = false;
			for (size_t i = 0; i < AUDIO_ACTIVE_SOUND_SIZE; i++) {
				if (!active_sounds[i]) {
					found_a_blank = true;
					first_free_slot = i;
					break;
				}
			}
			if (!found_a_blank) {
				log(
					"unable to play sound %s"
					" because of my stingy programmer that only gave me %zu slots",
					filename, AUDIO_ACTIVE_SOUND_SIZE
				);
				return;
			}
		}
		// sound->file
		sound->file = fopen(filename, "rb");
		if (!sound->file) {
			delete sound;
			log("can't open this file: %s", filename);
			return;
		}
		// sound->vf
		if (ov_open_callbacks(
			sound->file, &sound->vf,
			NULL, 0, OV_CALLBACKS_NOCLOSE
		) < 0) {
			delete sound;
			fclose(sound->file);
			log("can't open ogg vorbis file: %s", filename);
			return;
		}
		if (!sound->vf.seekable && loop) {
			log("unseekable file requested to be looped: %s", filename);
			log("Isn't that plain weird?");
		}
		// sound->*_head
		sound->play_head = 0;
		sound->load_head = 0;
		// etc.
		sound->eof = false;
		sound->loop = loop;
		// Fill in the blanks with the words you hear.
		active_sounds[first_free_slot] = sound;
		decode_vorbis(sound);
		// sound->decode_thread
		sound->decode_thread = new thread(decode_vorbis_thread, sound);
		ensure_no_error(Pa_StartStream(sound->stream));
	}
	//-------------------------------------------------------------------------
	// ● 扔掉active_sounds中已经播放完的条目
	//-------------------------------------------------------------------------
	void compact_channels() {
		for (size_t i = 0; i < AUDIO_CHANNELS_SIZE; i++) {
			if (!channels[i]) continue;
			if (!channels[i]->active) {
				//ov_clear(&active_sounds[i]->vf);
				//fclose(active_sounds[i]->file);
				//active_sounds[i]->decode_thread->join();
				//delete active_sounds[i]->decode_thread;
				delete channels[i];
				channels[i] = NULL;
			}
		}
	}
	//-------------------------------------------------------------------------
	// ● 播放声音的回调函数
	//-------------------------------------------------------------------------
	pa_callback(callback) {
		float* output = (float*) output_buffer;
		struct active_sound* sound = (struct active_sound*) user_data;
		while (frame_count > 0) {
			if (sound->play_head < sound->load_head) {
				size_t index = sound->play_head % AUDIO_VF_BUFFER_SIZE;
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
		size_t t;
		while ((t = sound->load_head - sound->play_head) < AUDIO_VF_BUFFER_SIZE) {
			// After ov_read_float(), tmp_buffer will be changed.
			float** tmp_buffer;
			long ret = ov_read_float(
				&sound->vf,
				&tmp_buffer,
				AUDIO_VF_BUFFER_SIZE - t,
				&sound->bitstream
			);
			if (ret > 0) {
				// On reading successful:
				for (long i = 0; i < ret; i++) {
					size_t j = (sound->load_head + i) % AUDIO_VF_BUFFER_SIZE;
					sound->vf_buffer[0][j] = tmp_buffer[0][i];
					sound->vf_buffer[1][j] = tmp_buffer[1][i];
				}
				sound->load_head += ret;
			} else if (ret == 0) {
				// On reached End Of File:
				if (sound->loop) {
					int ret = ov_pcm_seek(&sound->vf, 0);
					#define THE_SOUND "A sound stream played by play_loop "
					if (ret == OV_ENOSEEK) {
						log(THE_SOUND "is not seekable. (OV_ENOSEEK)");
					} else if (ret == OV_EREAD) {
						log(THE_SOUND "is not readable. (OV_EREAD)");
						return;
					} else if (ret == OV_EBADLINK) {
						log(THE_SOUND "may be corrupted. (OV_EBADLINK)");
					} else if (ret == OV_EFAULT) {
						log("You encountered a bug!");
						return;
					}
					// After this, we can go on ov_read'ing in the next loop.
					#undef THE_SOUND
				} else {
					while (sound->load_head - sound->play_head < AUDIO_VF_BUFFER_SIZE) {
						size_t j = sound->load_head % AUDIO_VF_BUFFER_SIZE;
						sound->vf_buffer[0][j] = .0f;
						sound->vf_buffer[1][j] = .0f;
						sound->load_head++;
					}
					sound->eof = true;
					return;
				}
			} else if (ret == OV_EBADLINK) {
				log("bad vorbis data (OV_EBADLINK)");
				return;
			} else if (ret == OV_EINVAL) {
				log("bad vorbis data (OV_EINVAL)");
				return;
			}
			// We must not free(tmp_buffer). It isn't ours.
		}
	}
	//-------------------------------------------------------------------------
	// ● 解码线程函数
	//-------------------------------------------------------------------------
	void decode_vorbis_thread(struct active_sound* sound) {
		while (!sound->eof) decode_vorbis(sound);
	}
}