//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   VMDE通用C++头文件。
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <ctime>
#include <thread>
#include <stdarg.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#define GLM_FORCE_SSE41
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <portaudio.h>
#include <vorbis/vorbisfile.h>

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	#include "VMDE.hpp"
	#ifdef __CYGWIN__
		#warning This will not work in Cygwin. Try at your own risk.
	#endif
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
	#define EXP_ASSIGNMENT ((void) (*((float*) NULL) = INFINITY))
	// EXP_MEMORY - EXPLOSION的标准C库内存操作实现
	#define EXP_MEMORY ((void) memset(NULL, 233, 1))
	// EXP_SIGNAL - EXPLOSION的标准C库信号实现
	// * 需要#include <csignal>，但本项目没有包含。
	#define EXP_SIGNAL ((void) ((raise(SIGSEGV) == 0) || abort()))
	// EXP_CONSTSTR - EXPLOSION的只读字符串实现
	#define EXP_CONSTSTR ((void) *"" = 0;)
	// EXP_ASM - EXPLOSION的Pentium Pro指令实现
	// * 这个实现不会导致SIGSEGV。
	#define EXP_ASM (__asm__("UD2"))
	// xeFree - 释放内存黑魔法
	#define xefree(pointer) \
		do { if (pointer) { \
		free(pointer); pointer = NULL; \
		} } while (false)
	//-------------------------------------------------------------------------
	// ● shaders.cpp
	//-------------------------------------------------------------------------
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;
		GLuint shaderProgram;

	public:
		Shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		void link_program();
		void use();
		void ProjectionView(glm::mat4 projection, glm::mat4 view);
	};

	//-------------------------------------------------------------------------
	// ● init.cpp
	//-------------------------------------------------------------------------
	void setup_viewport();
	void init_vmde(int w, int h);
	//-------------------------------------------------------------------------
	// ● main.cpp
	//-------------------------------------------------------------------------
	void glfw_error_callback(int error, const char* description);
	void main_draw_start();
	void main_draw_end();
	void main_set_brightness(float b);
	extern glm::mat4 projection, view;
	//-------------------------------------------------------------------------
	// ● resources.cpp
	//-------------------------------------------------------------------------
	namespace Res {
		class Texture {
		public:
			GLuint texture;
			int width, height;
			Texture(char* file, GLuint index);
		};
		extern Texture* tex_unit[16];
	}
	//-------------------------------------------------------------------------
	// ● util.cpp
	//-------------------------------------------------------------------------
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	// log - Util::log_internal的方便缩写，可以直接得到当前函数名
	//     log("%p", log);
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		extern const float PIf;
		extern const double PI;
		#define PId PI
		void log_internal(const char* function_name, const char* format, ...);
		char* read_file(const char* filename);
	}
	//-------------------------------------------------------------------------
	// ● VMDE
	//-------------------------------------------------------------------------
	struct VMDEState {
		bool frozen;
		float brightness;
	};

	struct VMDE {
		VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
		double frame_time;
	};

	// VMDE操控的全局变量
	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	//-------------------------------------------------------------------------
	// ● Audio
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
	// ● GDrawable
	//-------------------------------------------------------------------------
	class GDrawable {
	public:
		struct Data {
			GLfloat* vertices;
			int vtx_c;
			GLuint* indices;
			int ind_c;
			GLuint* mat;
			int mat_c;
			GLuint MBO;
			GLuint VAO;
			GLuint VBO;
			GLuint EBO;
			GLuint tri_mesh_count;
			glm::mat4 model;
		} data;

		void draw(GLuint start, GLuint end);
		void draw();
		void fbind();
		void update();
		void update_instance();
		void dispose();

		GDrawable();
	};
#endif
