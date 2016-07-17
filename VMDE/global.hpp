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

#ifndef _INCLUDE_GLOBAL_H
#define _INCLUDE_GLOBAL_H

	typedef VALUE (*type_ruby_function)(ANYARGS);
	typedef void (*type_free_function)(void*);
	#define UNUSED __attribute__((unused))
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
	#define TWICE \
		for (int _my_lovely_index = 0; _my_lovely_index < 2; _my_lovely_index++)

	// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	#ifdef __CYGWIN__
		#warning This wont work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif

	// GDrawable.cpp
	namespace GDrawable {
		struct GDrawable {
			GLfloat* vertices;
			GLuint VAO;
			GLuint VBO;
			size_t size_of_VBO;
		};
		void draw(GDrawable* s);
		void update(GDrawable* s);
		GDrawable* create();
	}

	// Global
	extern const char* GAME_NAME;

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

	struct RenderChainNode {
		RenderChainNode* prev;
		VALUE n;
		GDrawable::GDrawable* gd;
		bool visible;
		RenderChainNode* next;
	};

	#define RCN struct RenderChainNode

	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern VALUE ruby_VMDE;
	extern VALUE ruby_GResPic;
	extern VALUE ruby_GDrawable;
	extern RenderChainNode* render_chain;

	extern GLuint VBO[15];
	extern GLuint VAO[15];

	// shaders.cpp
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

	// ruby_connection.cpp
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

	// init.cpp
	void init_engine(int w, int h);
	void setup_viewport();
	void init_vmde(int w, int h);

	// main.cpp
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();
	void main_set_brightness(float b);

	void matrix2D();

	// resources.cpp
	int load_img(char* p);

	// util.cpp
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		extern const float PIf;
		extern const double PI;
		#define PId PI
		void log_internal(const char* function_name, const char* format, ...);
	}

	// audio.cpp
	namespace Audio {
		extern PaStream *stream;
		struct triangle_data {
			float value;
			float delta;
		};
		struct sine_data {
			float index;
			float index_delta;
			bool minus;
		};
		struct callback_data {
			double sample_rate;
			// type = 0……静音；1……三角波；2……正弦波；3……指定音频
			// 为啥不用枚举？因为太麻烦了！
			int type;
			union {
				struct triangle_data triangle;
				struct sine_data sine;
			} data;
		};
		extern struct callback_data data;
		extern float sine_table[256];
		extern const size_t sine_table_size;
		void init();
		void wobuzhidaozhegefangfayinggaijiaoshenmemingzi();
		void ensure_no_error(PaError err);
		int play_callback(
			const void* input_buffer UNUSED,
			void* output_buffer,
			unsigned long frames_per_buffer,
			const PaStreamCallbackTimeInfo* time_info UNUSED,
			PaStreamCallbackFlags status_flags UNUSED,
			void* user_data
		);
		void stop();
		void play_triangle(float freq);
		void get_next_triangle_value(struct triangle_data* data);
		void play_sine(float freq);
		void populate_sine_table();
		void get_next_sine_value(struct sine_data* data);
	}

#endif
