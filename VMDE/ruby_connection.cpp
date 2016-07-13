//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数，也负责一些Ruby扩展所需的杂务。
//=============================================================================

#include "global.hpp"

VALUE wrap_load_pic(VALUE self, VALUE path) {
	Check_Type(path, T_STRING);
	char* a = RSTRING_PTR(path);
	return Qnil;//RSTRING(load_img(a));
}

VALUE wrap_init_engine(VALUE self, VALUE w, VALUE h) {
	Check_Type(w, T_FIXNUM);
	Check_Type(h, T_FIXNUM);
	return INT2FIX(init_engine(FIX2INT(w), FIX2INT(h)));
}

VALUE wrap_main_draw_loop() {
	main_draw_loop();
	return Qnil;
}

VALUE wrap_main_get_frame_count() {
	return LONG2FIX(VMDE->frame_count);
}

VALUE wrap_main_get_fps() {
	return INT2FIX(VMDE->fps);
}

void init_RClass() {
	GResPic = rb_define_class_under(Global_module, "GRes_Picture", rb_cObject);
	rb_define_method(GResPic, "load_pic", (type_ruby_function) wrap_load_pic, 1);
}

void init_RModule() {
	Global_module = rb_define_module("VMDE");
	rb_define_module_function(Global_module, "init", (type_ruby_function) wrap_init_engine, 2);
	rb_define_module_function(Global_module, "update", (type_ruby_function) wrap_main_draw_loop, 0);
	rb_define_module_function(Global_module, "frame_count", (type_ruby_function) wrap_main_get_frame_count, 0);
	rb_define_module_function(Global_module, "fps", (type_ruby_function) wrap_main_get_fps, 0);
}

extern "C" DLLEXPORT void Init_VMDE() {
	log("initializing the module");

	init_RModule();
	init_RClass();
}
