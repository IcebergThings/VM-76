#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <ruby.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>

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
void glfw_error_callback();
void main_draw_loop();

#endif
