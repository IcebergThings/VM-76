//=============================================================================
// ■ channel_vorbis.cpp
//-----------------------------------------------------------------------------
//   提供解码Ogg Vorbis音频文件功能的声音通道。
//=============================================================================

#include "../global.hpp"

namespace Audio {
	void Channel_Vorbis::fill(float* buf, unsigned long n) {
	}
	Channel_Vorbis(const char* filename, bool _loop) {
		// f
		f = fopen(filename, "rb");
		if (!file) {
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
		// sound->*_head
		play_head = 0;
		load_head = 0;
		// etc.
		eof = false;
		loop = _loop;
		decode_vorbis();
		// decode_thread
		decode_thread = new thread(decode_vorbis_thread, sound);
	}
	~Channel_Vorbis() {
		delete decode_thread;
	}
}
