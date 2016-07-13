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
#define DBG_HEAD "VM DBG [C][VMDrawEngine]"
#define RB_F_R VALUE(*)(ANYARGS) // Ruby Function Return Type

// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
#ifdef __MINGW32__
#include <windows.h>
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
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

extern VMDE_t* VMDE;
extern GLFWwindow* window;
extern VALUE Global_module;
extern VALUE GResPic;

// ruby_connection.cpp
VALUE warp_load_pic(VALUE self, VALUE path);
VALUE wrap_init_engine(VALUE self, VALUE w, VALUE h);
VALUE warp_main_draw_loop();
VALUE warp_main_get_frame_count();
VALUE warp_main_get_fps();

// init.cpp
int init_engine(int w, int h);
void init_shaders();
void init_RClass();
void init_RModule();

// main.cpp
void glfw_error_callback(int error, const char* description);
void main_draw_loop();

// resources.cpp
int load_img(char* p);

#endif
