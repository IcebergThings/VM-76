#include "engine.hpp"
/*
VALUE warp_load_pic(VALUE self, VALUE path) {
	char* a;

	a = RSTRING(path);
	return RSTRING(load_img(a));
}*/

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

extern "C" __declspec(dllexport) void Init_VMDE() {
	printf(DBG_HEAD "[Init_VMDrawEngine]: MODULE INIT\n");

	Global_module = rb_define_module("VMDE");
	rb_define_module_function(Global_module, "init_engine", (RB_F_R) wrap_init_engine, 2);
	rb_define_module_function(Global_module, "draw_loop", (RB_F_R) warp_main_draw_loop, 0);
}
