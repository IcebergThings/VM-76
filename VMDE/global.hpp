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

#include <ruby.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	//-------------------------------------------------------------------------
	// ● 定义类型
	//-------------------------------------------------------------------------
	typedef VALUE (*type_ruby_function)(ANYARGS);
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
	// TWICE - 执行两次
	//     TWICE a++;
	//     TWICE { a++; b++; }
	#define TWICE \
		for (int _my_lovely_index = 0; _my_lovely_index < 2; _my_lovely_index++)
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
	// EXP_ - EXPLOSION的???实现
	// TODO：本来想再写一个实现的，结果写完上面两个就忘记了这个是什么了。
	#define EXP_
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __CYGWIN__
		#warning This wont work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif
	//-------------------------------------------------------------------------
	// ● GDrawable.cpp
	//-------------------------------------------------------------------------
	namespace GDrawable {
		struct GDrawable {
			GLfloat* vertices;
			GLuint VAO;
			GLuint VBO;
			GLuint tri_mesh_count;
			size_t size_of_VBO;
		};
		void draw(GDrawable* s);
		void fbind(GDrawable* s);
		void update(GDrawable* s);
		GDrawable* create();
	}
	//-------------------------------------------------------------------------
	// ● Global
	//-------------------------------------------------------------------------
	#define GAME_NAME "VM / 76"

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
	};

	#define RCN struct RenderChainNode
	struct RenderChainNode {
		RenderChainNode* prev;
		VALUE n;
		GDrawable::GDrawable* gd;
		bool visible;
		RenderChainNode* next;
	};

	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern VALUE ruby_VMDE;
	extern VALUE ruby_GResPic;
	extern VALUE ruby_GDrawable;
	extern RenderChainNode* render_chain;

	extern GLuint VBO[15];
	extern GLuint VAO[15];
	//-------------------------------------------------------------------------
	// ● shaders.cpp
	//-------------------------------------------------------------------------
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;
		GLuint shaderProgram;

	public:
		void init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		void link_program();
	};

	extern const GLchar* temp_vertexShaderSource;
	extern const GLchar* temp_fragmentShaderSource;

	extern Shaders* main_shader;
	//-------------------------------------------------------------------------
	// ● ruby_connection.cpp
	//-------------------------------------------------------------------------
	namespace RubyWrapper {
		VALUE load_pic(VALUE self, VALUE path);
		VALUE init_engine(VALUE self, VALUE w, VALUE h);
		VALUE main_draw_loop();
		VALUE main_get_frame_count();
		VALUE main_get_fps();
		VALUE main_matrix2D();
		VALUE main_set_brightness(VALUE self, VALUE b);
	}
	void init_ruby_classes();
	void init_ruby_modules();
	//-------------------------------------------------------------------------
	// ● init.cpp
	//-------------------------------------------------------------------------
	void init_engine(int w, int h);
	void setup_viewport();
	void init_vmde(int w, int h);
	//-------------------------------------------------------------------------
	// ● main.cpp
	//-------------------------------------------------------------------------
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();
	void main_set_brightness(float b);
	void matrix2D();
	//-------------------------------------------------------------------------
	// ● resources.cpp
	//-------------------------------------------------------------------------
	int load_img(char* p);
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
	}
	//-------------------------------------------------------------------------
	// ● audio.cpp and audio_waves.cpp
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
			// type = 0……静音；1……三角波；2……正弦波；3……指定音频；4……白噪音
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
#endif
