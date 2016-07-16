//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数，也负责一些Ruby扩展所需的杂务。
//=============================================================================

#include "global.hpp"

typedef void (*freefunc_t)(void*);

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

	struct ptr_data {
		void *ptr;
		long size;
		freefunc_t free;
		VALUE wrap[2];
	};

	VALUE gdrawable_visible_set(VALUE val, VALUE cdata) {
		return Qnil;
	}

	static void rb_data_mark(void *ptr) {
		struct ptr_data *data = ptr;
		if (data->wrap[0])
			rb_gc_mark(data->wrap[0]);
		if (data->wrap[1])
			rb_gc_mark(data->wrap[1]);
	}

	static void rb_data_free(void *ptr) {
		struct ptr_data *data = ptr;
		if (data->ptr && data->free)
			(*(data->free))(data->ptr);
		xfree(ptr);
	}

	static size_t rb_data_memsize(const void *ptr) {
		const struct ptr_data *data = ptr;
		return sizeof(*data) + data->size;
	}

	rb_data_type_t gdrawable_data_type = {
		"Drawable_C_Data",
		{rb_data_mark, rb_data_free, rb_data_memsize},
	};

	VALUE gdrawable_visible_get(VALUE val, VALUE cdata) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		log("%d", node);
		return node->visible ? Qtrue : Qfalse;
	}

	VALUE gdrawable_bind_obj(VALUE self) {

		RCN* node = (RCN*) malloc(sizeof(RCN));
		node->n = self;
		node->visible = true;
		node->prev = render_chain;
		if (node->prev == NULL)
			render_chain = node;
		else
			node->prev->next = node;
		node->next = NULL;
		node->gd = GDrawable::create();
		node->gd->vertices = (GLfloat*) malloc(sizeof(vertices));
		memcpy(node->gd->vertices, vertices, sizeof(vertices));
		node->gd->size_of_VBO = sizeof(vertices);
		GDrawable::update(node->gd);

		struct ptr_data* data;
		VALUE v = TypedData_Make_Struct(rb_cData, struct ptr_data, &gdrawable_data_type, data);
		data->ptr = node;
		data->free = free;
		data->size = sizeof(RCN);

		return v;
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
	ruby_VMDE = rb_define_module("VMDE");
	#define RUBY_MODULE_API(ruby_name, wrapper_name, parameter_count) \
		rb_define_module_function(ruby_VMDE, #ruby_name, \
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
	ruby_GResPic = rb_define_class_under(ruby_VMDE, "GResPic", rb_cObject);
	rb_define_method(ruby_GResPic, "load_pic", (type_ruby_function) RubyWrapper::load_pic, 1);

	ruby_GDrawable = rb_define_class_under(ruby_VMDE, "GDrawable", rb_cObject);
	rb_define_method(ruby_GDrawable, "bind", (type_ruby_function) RubyWrapper::gdrawable_bind_obj, 0);
	rb_define_method(ruby_GDrawable, "get_visible", (type_ruby_function) RubyWrapper::gdrawable_visible_get, 1);
}

EXPORTED void Init_VMDE() {
	log("initializing the module");

	init_ruby_modules();
	init_ruby_classes();
}
