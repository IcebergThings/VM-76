#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <ruby.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <string>

#ifndef _INCLUDE_ENGINE_H
#define _INCLUDE_ENGINE_H

#define GAME_NAME "VM / 76"
#define DBG_HEAD "VM DBG [C][VMDrawEngine]"
#define RB_F_R VALUE(*)(ANYARGS) // Ruby Function Return Type

// PY Deal For MICR0$0FT Windoges
// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#ifdef HAVE_WINDOWS_H
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Global
extern GLFWwindow* window;
extern VALUE Global_module;

// init.cpp
int init_engine(int w, int h);

// main.cpp
void glfw_error_callback(int error, const char* description);
void main_draw_loop();

// resources.cpp
int load_img(char* p);

#endif
