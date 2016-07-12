//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数，也负责一些Ruby扩展所需的杂务。
//=============================================================================

#include "global.hpp"

VALUE warp_load_pic(VALUE self, VALUE path) {
  char* a = RSTRING_PTR(path);

  return Qnil;//RSTRING(load_img(a));
}

VALUE wrap_init_engine(VALUE self, VALUE w, VALUE h) {
	int a, b;

	a = FIX2INT(w);
	b = FIX2INT(h);
	return INT2FIX(init_engine(a, b));
}

VALUE warp_main_draw_loop() {
	main_draw_loop();
	return Qnil;
}

VALUE warp_main_get_frame_count() {
  return LONG2FIX(VMDE->framecount);
}

VALUE warp_main_get_fps() {
  return INT2FIX(VMDE->fps);
}

extern "C" DLLEXPORT void Init_VMDE() {
  printf("%s[Init_VMDrawEngine]: MODULE INIT\n", DBG_HEAD);

  init_RModule();
  init_RClass();
}
