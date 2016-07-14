//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   VMDE通用C++头文件。
//=============================================================================

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ruby.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#ifndef _INCLUDE_GLOBAL_H
#define _INCLUDE_GLOBAL_H

	typedef VALUE (*type_ruby_function)(ANYARGS);

	// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	#ifdef __MINGW32__
	#include <windows.h>
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif

	// Global
	extern const char* GAME_NAME;

	struct VMDEState {
		bool frozen;
	};

	struct VMDE {
		struct VMDEState state;
		long frame_count;
		long millisecond;
		int fps;
	};

	typedef struct _Render_Chain_Node {
		struct _Render_Chain_Node* prev; // What the fuck is struct _Render_Chain?
		VALUE n;
		struct _Render_Chain_Node* next;
	} RenderChainNode;

	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern VALUE Global_module;
	extern VALUE GResPic;
	extern RenderChainNode* render_chain;

	extern GLuint VBO[15];

	// shaders.cpp
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;

	public:
		int init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		int use();
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
	}
	void init_ruby_classes();
	void init_ruby_modules();

	// init.cpp
	int init_engine(int w, int h);
	void setup_viewport();
	int init_shaders();
	void init_RClass();
	void init_RModule();

	// main.cpp
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();

	// resources.cpp
	int load_img(char* p);

	// util.cpp
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		void log_internal(const char* function_name, const char* format, ...);
	}

#endif
