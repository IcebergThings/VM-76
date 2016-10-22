//=============================================================================
// ■ ruby_connection.cpp
//-----------------------------------------------------------------------------
//   包装每一个导出到Ruby中的函数（即RubyWrapper），
//   还负责一些Ruby扩展所需的杂务和整个初始化系统的调用。
//=============================================================================

#include "VMDE.hpp"
#include "audio.hpp"
#include "GDrawable.hpp"

namespace RubyWrapper {

	struct ptr_data {
		void* ptr;
		long size;
		type_free_function free;
		VALUE wrap[2];
	};

	static void rb_data_mark(void* ptr) {
		struct ptr_data* data = (struct ptr_data*) ptr;
		if (data->wrap[0]) rb_gc_mark(data->wrap[0]);
		if (data->wrap[1]) rb_gc_mark(data->wrap[1]);
	}

	static void rb_data_free(void* ptr) {
		struct ptr_data* data = (struct ptr_data*) ptr;
		if (data->ptr && data->free) (*(data->free))(data->ptr);
		RCN* n = (RCN*) data->ptr;
		if (n) {
			GDrawable::dispose(n->gd);
		}
		xefree(ptr);
	}

	static size_t rb_data_memsize(const void* ptr) {
		const struct ptr_data* data = (const struct ptr_data*) ptr;
		return sizeof(*data) + data->size;
	}

	rb_data_type_t gdrawable_data_type = {
		"Drawable_C_Data",
		{rb_data_mark, rb_data_free, rb_data_memsize},
	};

	VALUE gdrawable_visible_get(VALUE self UNUSED, VALUE cdata) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		return node->visible ? Qtrue : Qfalse;
	}

	VALUE gdrawable_visible_set(VALUE self UNUSED, VALUE cdata, VALUE b) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		if (b == Qtrue && node->visible != true) {
			node->visible = true;
			node->prev = NULL;
			node->next = render_chain;
			if (node->next)
				node->next->prev = node;
			render_chain = node;
		}
		if (b == Qfalse && node->visible != false) {
			node->visible = false;
			if (node == render_chain)
				render_chain = node->next;
			if (node->prev)
				node->prev->next = node->next;
			if (node->next)
				node->next->prev = node->prev;
			node->prev = NULL;
			node->next = NULL;
		}

		return b;
	}

	size_t load_vertices(VALUE vertex_array, VALUE indices_array, GDrawable::GDrawable* gd) {
		Check_Type(vertex_array, T_ARRAY);
		long i;
		long length_v = rb_array_len(vertex_array);
		for (i = 0; i < length_v; i++) {
			VALUE k = rb_ary_entry(vertex_array, i);
			Check_Type(k, T_FLOAT);
		}
		long length_i = rb_array_len(indices_array);
		for (i = 0; i < length_i; i++) {
			VALUE k = rb_ary_entry(indices_array, i);
			Check_Type(k, T_FIXNUM);
		}

		size_t size = sizeof(GLfloat) * length_v;
		xefree(gd->vertices);
		gd->vertices = (GLfloat*) malloc(size);
		for (i = 0; i < length_v; i++) {
			gd->vertices[i] = (GLfloat) RFLOAT_VALUE(rb_ary_entry(vertex_array, i));
		}

		size = sizeof(GLuint) * length_i;
		xefree(gd->indices);
		gd->indices = (GLuint*) malloc(size);
		for (i = 0; i < length_i; i++) {
			gd->indices[i] = (GLuint) FIX2UINT(rb_ary_entry(indices_array, i));
		}

		gd->vtx_c = length_v;
		gd->ind_c = length_i;
		gd->tri_mesh_count = length_i / 3;
		return size;
	}

	VALUE gdrawable_bind_obj(VALUE self, VALUE aryv, VALUE aryi) {
		RCN* node = (RCN*) malloc(sizeof(RCN));
		node->n = self;
		node->visible = false; node->prev = NULL; node->next = NULL;
		node->gd = GDrawable::create();
		load_vertices(aryv, aryi, node->gd);
		GDrawable::fbind(node->gd);

		struct ptr_data* data;
		VALUE v = TypedData_Make_Struct(rb_cData, struct ptr_data, &gdrawable_data_type, data);
		data->ptr = node;
		data->free = free;
		data->size = sizeof(RCN);

		return v;
	}

	VALUE gdrawable_update_vertices(VALUE self UNUSED, VALUE cdata, VALUE aryv, VALUE aryi) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;

		//if (!node->gd) return Qfalse;

		load_vertices(aryv, aryi, node->gd);
		GDrawable::update(node->gd);

		return Qnil;
	}

	VALUE gdrawable_model_translate(VALUE self UNUSED, VALUE cdata, VALUE x, VALUE y, VALUE z) {
		Check_Type(x, T_FLOAT);
		Check_Type(y, T_FLOAT);
		Check_Type(z, T_FLOAT);

		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;
		glm::vec3 translate(RFLOAT_VALUE(x), RFLOAT_VALUE(y), RFLOAT_VALUE(z));
		node->gd->model = glm::translate(glm::mat4(1.0f), translate);

		return Qtrue;
	}

	VALUE gdrawable_render(VALUE self UNUSED, VALUE cdata) {
		struct ptr_data *data;
		(RCN*) TypedData_Get_Struct(cdata, ptr_data, &gdrawable_data_type, data);
		RCN* node = (RCN*) data->ptr;
		GDrawable::draw(node->gd, projection, view);

		return Qnil;
	}

	VALUE init_engine(VALUE self UNUSED, VALUE w, VALUE h) {
		Check_Type(w, T_FIXNUM);
		Check_Type(h, T_FIXNUM);
		::init_engine(FIX2INT(w), FIX2INT(h));
		return Qnil;
	}

	VALUE main_draw_init() {
		::main_draw_start();
		return Qnil;
	}

	VALUE main_draw_end() {
		::main_draw_end();
		return Qnil;
	}

	VALUE main_get_frame_count() {
		return LONG2FIX(VMDE->frame_count);
	}

	VALUE main_get_fps() {
		return INT2FIX(VMDE->fps);
	}

	VALUE main_get_frame_time() {
		return rb_float_new(VMDE->frame_time);
	}

	VALUE main_matrix2D() {
		::matrix2D();
		return Qtrue;
	}

	VALUE ruby_reload_shaders() {
		reload_shaders();
		return Qnil;
	}

	VALUE load_tex(VALUE self UNUSED, VALUE index, VALUE file) {
		Check_Type(index, T_FIXNUM);
		Check_Type(file, T_STRING);
		char* filestr = rb_string_value_ptr(&file);
		new Res::Texture(filestr, FIX2INT(index), main_shader->shaderProgram);
		//xefree(filestr);
		return Qtrue;
	}

	VALUE unload_tex(VALUE self UNUSED, VALUE index) {
		Check_Type(index, T_FIXNUM);
		if (index < 16 && index >= 0 && Res::tex_unit[index]) {
			// FIXME: How to disable that?
			// glRemoveTexture(GL_TEXTURE0 + index);
			free(Res::tex_unit[index]);
			Res::tex_unit[index] = NULL;
			return Qtrue;
		} else {
			return Qfalse;
		}
	}

	VALUE main_set_brightness(VALUE self UNUSED, VALUE value) {
		Check_Type(value, T_FLOAT);
		VMDE->state.brightness = RFLOAT_VALUE(value);
		return Qnil;
	}

	VALUE audio_stop(VALUE self UNUSED) {
		Audio::stop();
		return Qnil;
	}

	VALUE audio_play_wave(VALUE self UNUSED, VALUE type, VALUE freq) {
		Check_Type(type, T_SYMBOL);
		Check_Type(freq, T_FLOAT);
		float f = RFLOAT_VALUE(freq);
		if (f <= 0) rb_raise(rb_eRangeError, "frequency must be positive");
		ID type_id = SYM2ID(type);
		if (type_id == rb_intern("triangle")) {
			Audio::play_triangle(f);
		} else if (type_id == rb_intern("sine")) {
			Audio::play_sine(f);
		} else {
			rb_raise(rb_eArgError, "invalid wave type");
		}
		return Qnil;
	}

	VALUE audio_play_sound(VALUE self UNUSED, VALUE filename) {
		Check_Type(filename, T_STRING);
		char* c_filename = StringValueCStr(filename);
		Audio::play_sound(c_filename, false);
		return Qnil;
	}

	VALUE audio_play_loop(VALUE self UNUSED, VALUE filename) {
		Check_Type(filename, T_STRING);
		char* c_filename = StringValueCStr(filename);
		Audio::play_sound(c_filename, true);
		return Qnil;
	}
}

void init_ruby_modules() {
	#define RUBY_MODULE_API(module, ruby_name, wrapper_name, parameter_count) \
		rb_define_module_function(ruby_##module, #ruby_name, \
		(type_ruby_function) RubyWrapper::wrapper_name, parameter_count)
	ruby_VMDE = rb_define_module("VMDE");
	RUBY_MODULE_API(VMDE, init, init_engine, 2);
	RUBY_MODULE_API(VMDE, prepare, main_draw_init, 0);
	RUBY_MODULE_API(VMDE, update, main_draw_end, 0);
	RUBY_MODULE_API(VMDE, frame_count, main_get_frame_count, 0);
	RUBY_MODULE_API(VMDE, fps, main_get_fps, 0);
	RUBY_MODULE_API(VMDE, frame_time, main_get_frame_time, 0);
	RUBY_MODULE_API(VMDE, matrix2D, main_matrix2D, 0);
	RUBY_MODULE_API(VMDE, brightness=, main_set_brightness, 1);
	RUBY_MODULE_API(VMDE, reload_shaders, ruby_reload_shaders, 0);
	RUBY_MODULE_API(VMDE, load_tex, load_tex, 2);

	VALUE ruby_Audio = rb_define_module_under(ruby_VMDE, "Audio");
	RUBY_MODULE_API(Audio, stop, audio_stop, 0);
	RUBY_MODULE_API(Audio, play_wave, audio_play_wave, 2);
	RUBY_MODULE_API(Audio, play_sound, audio_play_sound, 1);
	RUBY_MODULE_API(Audio, play_loop, audio_play_loop, 1);
	#undef RUBY_MODULE_API
}

void init_ruby_classes() {
	ruby_GDrawable = rb_define_class_under(ruby_VMDE, "GDrawable", rb_cObject);
	rb_define_method(ruby_GDrawable, "bind", (type_ruby_function) RubyWrapper::gdrawable_bind_obj, 2);
	rb_define_method(ruby_GDrawable, "set_visible", (type_ruby_function) RubyWrapper::gdrawable_visible_set, 2);
	rb_define_method(ruby_GDrawable, "get_visible", (type_ruby_function) RubyWrapper::gdrawable_visible_get, 1);
	rb_define_method(ruby_GDrawable, "update_vertices", (type_ruby_function) RubyWrapper::gdrawable_update_vertices, 3);
	rb_define_method(ruby_GDrawable, "model_translate", (type_ruby_function) RubyWrapper::gdrawable_model_translate, 4);
	rb_define_method(ruby_GDrawable, "render", (type_ruby_function) RubyWrapper::gdrawable_render, 1);
}

EXPORTED void Init_VMDE() {
	log("initializing the module");

	init_ruby_modules();
	init_ruby_classes();
}
