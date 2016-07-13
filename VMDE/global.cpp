//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   VMDE范围内的全局变量表。
//=============================================================================

#include "global.hpp"

GLFWwindow* window;
VALUE Global_module;
VALUE GResPic;

VMDE_t* VMDE;

Render_Chain_Node Render_Chain;

GLuint VBO[15];
GLuint basic_2D_vsh;
GLuint basic_2D_fsh;
