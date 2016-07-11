#include <GLFW/glfw3.h>
#include <ruby.h>

#include <stdlib.h>
#include <stdio.h>

#ifndef _INCLUDE_ENGINE_H
#define _INCLUDE_ENGINE_H

#define GAME_NAME "VM / 76"
#define DBG_HEAD "VM DBG [C][VMDrawEngine]"

// Global
extern GLFWwindow* window;
extern VALUE Global_module;

// init.cpp
int init_engine(int w, int h);

// main.cpp
void main_draw_loop();

#endif
