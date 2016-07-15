//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   VMDE通用C++头文件。
//=============================================================================

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ruby.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "GDrawable.hpp"

#ifndef _INCLUDE_GLOBAL_H
#define _INCLUDE_GLOBAL_H

	typedef VALUE (*type_ruby_function)(ANYARGS);
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))

	// PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	// Becuase it;s Windoges, I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	#ifdef __CYGWIN__
		#warning This wont work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif

	// GDrawable.cpp
	namespace GDrawable {
		struct GDrawable {
			GLfloat* vertices;
			GLuint VAO;
			GLuint VBO;
		};
		void draw(GDrawable* s);
		void update(GDrawable* s);
		GDrawable* create();
	}

	// Global
	extern const char* GAME_NAME;

	struct VMDEState {
		bool frozen;
		float brightness;
	};

	struct VMDE {
		VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
	};

	struct RenderChainNode {
		RenderChainNode* prev;
		VALUE n;
		GDrawable::GDrawable* gd;
		RenderChainNode* next;
	};

	#define RCN struct RenderChainNode

	extern struct VMDE* VMDE;
	extern GLFWwindow* window;
	extern VALUE ruby_VMDE;
	extern VALUE ruby_GResPic;
	extern VALUE ruby_GDrawable;
	extern RenderChainNode* render_chain;

	extern GLuint VBO[15];
	extern GLuint VAO[15];

	// shaders.cpp
	class Shaders {
	public:
		GLuint basic_2D_vsh;
		GLuint basic_2D_fsh;
		GLuint shaderProgram;

	public:
		void init_shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr);
		void link_program();
	};

	extern const GLchar* temp_vertexShaderSource;
	extern const GLchar* temp_fragmentShaderSource;

	extern Shaders* main_shader;

	// ruby_connection.cpp
	namespace RubyWrapper {
		VALUE load_pic(VALUE self, VALUE path);
		VALUE init_engine(VALUE self, VALUE w, VALUE h);
		VALUE main_draw_loop();
		VALUE main_get_frame_count();
		VALUE main_get_fps();
		VALUE main_matrix2D();
		VALUE main_set_brightness(VALUE self, VALUE b);
	}
	void init_ruby_classes();
	void init_ruby_modules();

	// init.cpp
	void init_engine(int w, int h);
	void setup_viewport();

	// main.cpp
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();
	void main_set_brightness(float b);

	void matrix2D();

	// resources.cpp
	int load_img(char* p);

	// util.cpp
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		void log_internal(const char* function_name, const char* format, ...);
	}

#endif
