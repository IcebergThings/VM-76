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
	// 活动（正在播放中）的声音列表
	struct active_sound* active_sounds[AUDIO_ACTIVE_SOUND_SIZE] = {NULL};
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		ensure_no_error(Pa_Initialize());
		init_waves();
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
		if (!sound->vf.seekable && loop) {
			log("unseekable file requested to be looped: %s", filename);
			log("Isn't that plain weird?");
		}
		// sound->stream
		ensure_no_error(Pa_OpenDefaultStream(
			&sound->stream, 0, 2, paFloat32, 44100,
			256, play_sound_callback, sound
		));
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
	void compact_active_sounds_array() {
		for (size_t i = 0; i < AUDIO_ACTIVE_SOUND_SIZE; i++) {
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
						rb_raise(rb_eIOError, THE_SOUND "is not readable. (OV_EREAD)");
					} else if (ret == OV_EBADLINK) {
						log(THE_SOUND "may be corrupted. (OV_EBADLINK)");
					} else if (ret == OV_EFAULT) {
						rb_raise(rb_eRuntimeError, "You encountered a bug!");
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
				rb_raise(rb_eRuntimeError, "bad vorbis data (OV_EBADLINK)");
			} else if (ret == OV_EINVAL) {
				rb_raise(rb_eRuntimeError, "bad vorbis data (OV_EINVAL)");
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
