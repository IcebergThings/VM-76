//=============================================================================
// ■ global.cpp
//-----------------------------------------------------------------------------
//   VMDE范围内的全局变量表。
//=============================================================================

#include "VMDE.hpp"
#include "GDrawable.hpp"

GLFWwindow* window;
VALUE ruby_VMDE;
VALUE ruby_GResPic;
VALUE ruby_GDrawable;

struct VMDE* VMDE;

RenderChainNode* render_chain = NULL;
