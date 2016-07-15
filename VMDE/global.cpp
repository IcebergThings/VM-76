//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   VMDE范围内的全局变量表。
//=============================================================================

#include "global.hpp"

const char* GAME_NAME = "VM / 76";

GLFWwindow* window;
VALUE ruby_VMDE;
VALUE ruby_GResPic;
VALUE ruby_GDrawable;

struct VMDE* VMDE;

struct RenderChainNode* render_chain = NULL;

GLuint VBO[15];
GLuint VAO[15];
