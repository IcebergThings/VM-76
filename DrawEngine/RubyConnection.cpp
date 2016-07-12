#include "engine.hpp"

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

extern "C" DLLEXPORT void Init_VMDE() {
  printf("%s[Init_VMDrawEngine]: MODULE INIT\n", DBG_HEAD);

  init_RModule();
  init_RClass();
}
