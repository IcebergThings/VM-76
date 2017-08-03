//=============================================================================
// ■ CommandList.cpp
//-----------------------------------------------------------------------------
//   VMDE中渲染指令列表。
//=============================================================================

#include "global.hpp"

static uint32_t CL_GDrawable_render(uint8_t* input) {
	GDrawable* ptr = *((GDrawable**) input);
	ptr->render();

	return 0x0;
}

static uint32_t CL_GDrawable_renderOnce(uint8_t* input) {
	GDrawable* ptr = *((GDrawable**) input);
	ptr->renderOnce();

	return 0x0;
}

static uint32_t CL_GDrawable_batch(uint8_t* input) {
	GDrawable** ind = (GDrawable**) input;

	while (*ind != NULL) {
		GDrawable* ptr = *ind;
		ptr->render();

		ind++;
	}

	return 0x0;
}

static uint32_t CL_GDrawable_batchOnce(uint8_t* input) {
	GDrawable** ind = (GDrawable**) input;

	while (*ind != NULL) {
		GDrawable* ptr = *ind;
		ptr->renderOnce();

		ind++;
	}

	return 0x0;
}

static ASM76::BIOS_call func_list[] = {
	#define add(x) &CL_##x,
	#include "CommandList.hpp"
	#undef add
};
static int function_count = ARRAY_SIZE(func_list) + 1;

ASM76::BIOS* CmdList::bios;

void CmdList::global_init() {
	bios = new ASM76::BIOS(function_count);

	memcpy(bios->function_table, func_list, sizeof(func_list));
}

CmdList::CmdList(ASM76::Program p) {
	vm = new ASM76::VM(p);
	vm->firmware = bios;
}

void CmdList::call() {
	vm->execute_from(0x0, false);
}

CmdList::~CmdList() {
	XE(delete, vm);
}
