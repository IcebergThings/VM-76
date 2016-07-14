//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   VMDE范围内的全局变量表。
//=============================================================================

#include "global.hpp"

const char* GAME_NAME = "VM / 76";

GLFWwindow* window;
VALUE Global_module;
VALUE GResPic;

struct VMDE* VMDE;

RenderChainNode* render_chain = NULL;

GLuint VBO[15];
