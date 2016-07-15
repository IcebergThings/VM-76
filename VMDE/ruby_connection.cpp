//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数，也负责一些Ruby扩展所需的杂务。
//=============================================================================

#include "global.hpp"

GLfloat vertices[] = {
	0.0f, 0.0f, 0.0f,
	435.0f, 0.0f, 0.0f,
	435.0f, 270.0f, 0.0f
};

namespace RubyWrapper {
	VALUE load_pic(VALUE self, VALUE path) {
		Check_Type(path, T_STRING);
		char* a = RSTRING_PTR(path);
		return Qnil;//RSTRING(load_img(a));
	}

	VALUE gdrawable_visible_equ(VALUE self, VALUE s) {

	}

	VALUE gdrawable_bind_obj(VALUE self) {

		RCN *node = (RCN *) malloc(sizeof(RCN));
		node->n = self;
		node->prev = render_chain;
		if (node->prev == NULL)
			render_chain = node;
		else
			node->prev->next = node;
		node->next = NULL;
		node->gd = GDrawableNS::create();
		node->gd->vertices = (GLfloat*) malloc(sizeof(vertices));
		memcpy(node->gd->vertices, vertices, sizeof(vertices));
		node->gd->size_of_VBO = sizeof(vertices);
		GDrawableNS::update(node->gd);

		rb_data_type_t data_type = {
			"Drawable_C_Data",
			{
				0, 0,
			},
			0, (void*) node, RUBY_TYPED_FREE_IMMEDIATELY,
		};

		return TypedData_Wrap_Struct(rb_cData, &data_type, node);
	}

	VALUE init_engine(VALUE self, VALUE w, VALUE h) {
		Check_Type(w, T_FIXNUM);
		Check_Type(h, T_FIXNUM);
		::init_engine(FIX2INT(w), FIX2INT(h));
		return Qnil;
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

	VALUE main_matrix2D() {
		::matrix2D();
		return Qtrue;
	}

	VALUE main_set_brightness(VALUE self, VALUE b) {
		Check_Type(b, T_FLOAT);
		::main_set_brightness(RFLOAT_VALUE(b));
		return Qnil;
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
	RUBY_MODULE_API(matrix2D, main_matrix2D, 0);
	RUBY_MODULE_API(set_brightness, main_set_brightness, 1);

	#undef RUBY_MODULE_API
}

void init_ruby_classes() {
	GResPic = rb_define_class_under(Global_module, "GRes_Picture", rb_cObject);
	rb_define_method(GResPic, "load_pic", (type_ruby_function) RubyWrapper::load_pic, 1);

	GDrawable = rb_define_class_under(Global_module, "GDrawable", rb_cObject);
	rb_define_method(GDrawable, "bind", (type_ruby_function) RubyWrapper::gdrawable_bind_obj, 0);

}

EXPORTED void Init_VMDE() {
	log("initializing the module");

	init_ruby_modules();
	init_ruby_classes();
}
