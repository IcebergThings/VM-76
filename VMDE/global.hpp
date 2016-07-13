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
typedef VALUE (*type_ruby_function)(ANYARGS) ;

// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
#ifdef __MINGW32__
#include <windows.h>
#define EXPORTED extern "C" __declspec(dllexport)
#else
#define EXPORTED extern "C"
#endif

// Global
typedef struct _VMDE_State_t {
	bool freeze;
} VMDE_State_t;

typedef struct _VMDE_t {
	VMDE_State_t* States;
	long frame_count;
	long millisecond;
	int fps;
} VMDE_t;

typedef struct _Render_Chain_Node {
	struct _Render_Chain* prev;
	VALUE n;
	struct _Render_Chain* next;
} Render_Chain_Node;

extern VMDE_t* VMDE;
extern GLFWwindow* window;
extern VALUE Global_module;
extern VALUE GResPic;
extern Render_Chain_Node Render_Chain;

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
