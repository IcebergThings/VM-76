//=============================================================================
// ■ channel_vorbis.cpp
//-----------------------------------------------------------------------------
//   提供解码Ogg Vorbis音频文件功能的声音通道。
//=============================================================================

#include "../global.hpp"

namespace Audio {
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	Channel_Vorbis::Channel_Vorbis(const char* filename, bool _loop) {
		// f
		f = fopen(filename, "rb");
		if (!f) {
			log("can't open this file: %s", filename);
			return;
		}
		// vf
		if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
			fclose(f);
			log("can't open ogg vorbis file: %s", filename);
			return;
		}
		if (!vf.seekable && loop) {
			log("unseekable file requested to be looped: %s", filename);
			log("Isn't that plain weird?");
		}
		// etc.
		eof = false;
		loop = _loop;
		decode();
		// thrd
		thrd = new thread(decode_thread, this);
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	Channel_Vorbis::~Channel_Vorbis() {
		ov_clear(&vf);
		fclose(f);
		eof = true; // to make the thread stop
		thrd->join();
		delete thrd;
	}
	//-------------------------------------------------------------------------
	// ● 填充缓冲区
	//-------------------------------------------------------------------------
	void Channel_Vorbis::fill(float* buf, unsigned long n) {
		while (n) {
			if (vf_buf.is_empty()) {
				FEED_AUDIO_DATA(buf, .0f);
			} else {
				float a = vf_buf.dequeue();
				float b = vf_buf.dequeue();
				*buf++ = a;
				*buf++ = b;
			}
			n--;
		}
		if (vf_buf.is_empty() && eof) active = false;
	}
	//-------------------------------------------------------------------------
	// ● 解码文件来填vf_buf
	//-------------------------------------------------------------------------
	void Channel_Vorbis::decode() {
		size_t t;
		// 在这个除以2（因为立体声）上被坑了半个小时 —— satgo
		while ((t = vf_buf.remaining() / 2) > 0) {
			// After ov_read_float(), tmp_buffer will be changed.
			float** tmp_buffer;
			long ret = ov_read_float(
				&vf,
				&tmp_buffer,
				t,
				&bitstream
			);
			//log("%zu", vf_buf.remaining());
			if (ret > 0) {
				// On reading successful:
				for (long i = 0; i < ret; i++) {
					vf_buf.enqueue(tmp_buffer[0][i]);
					vf_buf.enqueue(tmp_buffer[1][i]);
				}
			} else if (ret == 0) {
				// On reached End Of File:
				if (loop) {
					int ret = ov_pcm_seek(&vf, 0);
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
					eof = true;
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
	void Channel_Vorbis::decode_thread(Channel_Vorbis* ch) {
		while (!ch->eof) ch->decode();
	}
}
