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
#include <string>

#include <ctime>

#ifndef _INCLUDE_ENGINE_H
#define _INCLUDE_ENGINE_H

#define GAME_NAME "VM / 76"
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
struct VMDEState {
	bool frozen;
};

struct VMDE {
	struct VMDEState state;
	long frame_count;
	long millisecond;
	int fps;
};

struct RenderChainNode {
	struct _Render_Chain* prev; // What the fuck is struct _Render_Chain?
	VALUE n;
	struct _Render_Chain* next;
};

extern struct VMDE* VMDE;
extern GLFWwindow* window;
extern VALUE Global_module;
extern VALUE GResPic;
extern RenderChainNode render_chain;

extern GLuint VBO[15];
extern GLuint basic_2D_vsh;

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
void init_engine(int w, int h);
void setup_viewport();
void init_shaders();

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
