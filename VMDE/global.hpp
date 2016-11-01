//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   在VMDE的内部，包容一切的头。
//   注意：本头不是用于上层程序调用的，此情形下请用VMDE.hpp。
//   关于只有一个头：
//   我完全无法理解为什么C要采用有头文件的设计。当然这很可能是历史遗留问题，
//   但这导致了C程序员总是需要花费大量时间整理头文件，且这时间花得莫名其妙。
//   而这在其他任何语系中都不是问题：即使是采用C语法的衍生语言，也不会保留
//   这个“特性”。因此我们需要解决它。
//   采用#include管理多文件编译是最好的解决方案。在现今编译单个文件的速度有极
//   大提升的时代，一个个编译小文件却慢得多。然而这个解决方案并不能在像VMDE这
//   样的合作项目中进行，因为它会让很多已建立的条件反射、编译器、IDE日狗。
//   单一头文件也是一个可行的方案。它保留了头文件，但只有一个——这意味着所有
//   你需要编辑的头都在固定的位置，给身体和手带来了放松的空间，同时也保证编辑
//   器的标签栏（如果有的话）不会因为头文件而缩小为原来的一半。
//   当然，还有一个解决办法就是只用一个.c文件。结果：从此这个项目就没落了。
//   综上所述，我选择狗带，砍了其他所有头。
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <ctime>
#include <cstdarg>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SSE41
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <portaudio.h>
#include <vorbis/vorbisfile.h>

#include "VMDE.hpp"

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	//-------------------------------------------------------------------------
	// ● 定义类型
	//-------------------------------------------------------------------------
	typedef void (*type_free_function)(void*);
	//-------------------------------------------------------------------------
	// ● 定义宏魔法
	//-------------------------------------------------------------------------
	// UNUSED - 标记无用函数参数
	//     int random(int n UNUSED) { return 4; }
	#define UNUSED __attribute__((unused))
	// ARRAY_SIZE - 获取定义的数组大小
	//     int a[56]; → ARRAY_SIZE(a) == 56
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
	// MLVN - Macro-Local Variable Name
	// 这是为了不让明文变量名被莫名其妙使用而设计的。
	//     #define TEST do { int MLVN() = 0, MLVN(another) = 0; } while(0)
	#define MLVN(name) My_Lovely_##name##___
	// NTIMES - 执行多次
	//     NTIMES(3) puts("Three or more, use a for");
	#define NTIMES(n) \
		for (unsigned short MLVN(index) = 0; MLVN(index) < (n); MLVN(index)++)
	// TWICE - 执行两次
	//     TWICE a++;
	//     TWICE { a++; b++; }
	#define TWICE NTIMES(2)
	// EXPLOSION!
	// 可以安全地在用户级代码中调用并获得SIGSEGV。
	// 在退出程序时装逼。
	#define EXPLOSION EXP_ASSIGNMENT
	// EXP_ASSIGNMENT - EXPLOSION的赋值实现
	#define EXP_ASSIGNMENT (*((float*) NULL) = INFINITY)
	// EXP_MEMORY - EXPLOSION的标准C库内存操作实现
	#define EXP_MEMORY (memset(NULL, 233, 1))
	// EXP_SIGNAL - EXPLOSION的标准C库信号实现
	// * 需要#include <csignal>，但本项目没有包含。
	#define EXP_SIGNAL ((raise(SIGSEGV) == 0) || abort())
	// EXP_CONSTSTR - EXPLOSION的只读字符串实现
	#define EXP_CONSTSTR (*"" = '*')
	// EXP_ASM - EXPLOSION的Pentium Pro指令实现
	// * 这个实现不会导致SIGSEGV。
	#define EXP_ASM (__asm__("ud2"))
	// xeFree - 释放内存黑魔法
	#define xefree(pointer) do { if (pointer) free(pointer); } while (false)
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __CYGWIN__
		#warning This will not work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif
	//-------------------------------------------------------------------------
	// ● audio.cpp & audio_waves.cpp
	//-------------------------------------------------------------------------
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
	//-------------------------------------------------------------------------
	// ● 
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● 
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	// ● 
	//-------------------------------------------------------------------------
#endif
