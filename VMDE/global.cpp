//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   VMDE范围内的全局变量表。
//=============================================================================

#include "global.hpp"

GLFWwindow* window;
VALUE Global_module;
VALUE GResPic;

struct VMDE* VMDE;

struct RenderChainNode render_chain;

GLuint VBO[15];
GLuint basic_2D_vsh;
