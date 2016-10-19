//=============================================================================
// ■ global.hpp
//-----------------------------------------------------------------------------
//   VMDE通用C++头文件。
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <ctime>
#include <thread>

#include <ruby.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <portaudio.h>
#include <vorbis/vorbisfile.h>
#include <SOIL.h>

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	//-------------------------------------------------------------------------
	// ● 定义类型
	//-------------------------------------------------------------------------
	typedef VALUE (*type_ruby_function)(ANYARGS);
	typedef void (*type_free_function)(void*);
	//-------------------------------------------------------------------------
	// ● 定义宏魔法
	//-------------------------------------------------------------------------
	// UNUSED - 标记无用函数参数
	//     int random(int n UNUSED) { return 4; }
	#define UNUSED __attribute__((unused))
	// ARRAY_SIZE - 获取定义的数组大小
	//     int a[56]; → ARRAY_SIZE(a) == 56
	#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
	// MLVN - Macro-Local Variable Name
	// 这是为了不让明文变量名被莫名其妙使用而设计的。
	//     #define TEST do { int MLVN() = 0, MLVN(another) = 0; } while(0)
	#define MLVN(name) My_Lovely_##name##___
	// NTIMES - 执行多次
	//     NTIMES(3) puts("Three or more, use a for");
	#define NTIMES(n) \
		for (unsigned short MLVN(index) = 0; MLVN(index) < (n); MLVN(index)++)
	// TWICE - 执行两次
	//     TWICE a++;
	//     TWICE { a++; b++; }
	#define TWICE NTIMES(2)
	// EXPLOSION!
	// 可以安全地在用户级代码中调用并获得SIGSEGV。
	// 在退出程序时装逼。
	#define EXPLOSION EXP_ASSIGNMENT
	// EXP_ASSIGNMENT - EXPLOSION的赋值实现
	#define EXP_ASSIGNMENT ((void) (*((float*) NULL) = INFINITY))
	// EXP_MEMORY - EXPLOSION的标准C库内存操作实现
	#define EXP_MEMORY ((void) memset(NULL, 233, 1))
	// EXP_SIGNAL - EXPLOSION的标准C库信号实现
	// * 需要#include <csignal>，但本项目没有包含。
	#define EXP_SIGNAL ((void) ((raise(SIGSEGV) == 0) || abort()))
	// EXP_CONSTSTR - EXPLOSION的只读字符串实现
	#define EXP_CONSTSTR ((void) *"" = 0;)
	// EXP_ASM - EXPLOSION的Pentium Pro指令实现
	// * 这个实现不会导致SIGSEGV。
	#define EXP_ASM (__asm__("UD2"))
	// xeFree - 释放内存黑魔法
	#define xefree(pointer) if (pointer) free(pointer)
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __CYGWIN__
		#warning This wont work in Cygwin. Try at your own risk.
	#endif
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif

	//-------------------------------------------------------------------------
	// ● Global
	//-------------------------------------------------------------------------
	#define GAME_NAME "VM / 76"

	//-------------------------------------------------------------------------
	// ● shaders.cpp
	//-------------------------------------------------------------------------
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
	//-------------------------------------------------------------------------
	// ● ruby_connection.cpp
	//-------------------------------------------------------------------------
	void init_ruby_classes();
	void init_ruby_modules();
	//-------------------------------------------------------------------------
	// ● init.cpp
	//-------------------------------------------------------------------------
	void init_engine(int w, int h);
	void setup_viewport();
	void init_vmde(int w, int h);
	//-------------------------------------------------------------------------
	// ● main.cpp
	//-------------------------------------------------------------------------
	void glfw_error_callback(int error, const char* description);
	void main_draw_loop();
	void main_set_brightness(float b);
	void matrix2D();
	//-------------------------------------------------------------------------
	// ● resources.cpp
	//-------------------------------------------------------------------------
	namespace Res {
		class Texture {
		private:
			GLuint texture;
		public:
			int width, height;
			Texture(char* file, GLuint index, Shaders sh);
		};

		extern Texture* tex_unit[16];
	}
	//-------------------------------------------------------------------------
	// ● util.cpp
	//-------------------------------------------------------------------------
	#define DEBUG_ENVIRONMENT "VMDrawEngine"
	// log - Util::log_internal的方便缩写，可以直接得到当前函数名
	//     log("%p", log);
	#define log(...) Util::log_internal(__func__, __VA_ARGS__)
	namespace Util {
		extern const float PIf;
		extern const double PI;
		#define PId PI
		void log_internal(const char* function_name, const char* format, ...);
	}
#endif
