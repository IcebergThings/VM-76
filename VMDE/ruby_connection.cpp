//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数，也负责一些Ruby扩展所需的杂务。
//=============================================================================

#include "global.hpp"

namespace RubyWrapper {
	VALUE load_pic(VALUE self, VALUE path) {
		Check_Type(path, T_STRING);
		char* a = RSTRING_PTR(path);
		return Qnil;//RSTRING(load_img(a));
	}

	VALUE init_engine(VALUE self, VALUE w, VALUE h) {
		Check_Type(w, T_FIXNUM);
		Check_Type(h, T_FIXNUM);
		::init_engine(FIX2INT(w), FIX2INT(h));
		return Qtrue;
	}

	VALUE main_draw_loop() {
		::main_draw_loop();
		return Qnil;
	}

	VALUE main_get_frame_count() {
		return LONG2FIX(VMDE->frame_count);
	}

	VALUE main_get_fps() {
		return INT2FIX(VMDE->fps);
	}
}

void init_ruby_modules() {
	Global_module = rb_define_module("VMDE");
	#define RUBY_MODULE_API(ruby_name, wrapper_name, parameter_count) \
		rb_define_module_function(Global_module, #ruby_name, \
		(type_ruby_function) RubyWrapper::wrapper_name, parameter_count)
	RUBY_MODULE_API(init, init_engine, 2);
	RUBY_MODULE_API(update, main_draw_loop, 0);
	RUBY_MODULE_API(frame_count, main_get_frame_count, 0);
	RUBY_MODULE_API(fps, main_get_fps, 0);
	#undef RUBY_MODULE_API
}

void init_ruby_classes() {
	GResPic = rb_define_class_under(Global_module, "GRes_Picture", rb_cObject);
	rb_define_method(GResPic, "load_pic", (type_ruby_function) RubyWrapper::load_pic, 1);
}

EXPORTED void Init_VMDE() {
	log("initializing the module");

	init_ruby_modules();
	init_ruby_classes();
}
