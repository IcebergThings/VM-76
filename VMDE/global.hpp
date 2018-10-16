//=============================================================================
// ■ VMDE/global.hpp
//-----------------------------------------------------------------------------
//   在VMDE的内部，包容一切的头。
//   关于只有一个头：
//   我完全无法理解为什么C要采用有头文件的设计。当然这很可能是历史遗留问题，
//   但这导致了C程序员总是需要花费大量时间整理头文件，且这时间花得莫名其妙。
//   而这在其他任何语系中都不是问题：即使是采用C语法的衍生语言，也不会保留
//   这个“特性”。因此我们需要解决它。
//   采用#include管理多文件编译是最好的解决方案。在现今编译单个文件的速度有极
//   大提升的时代，一个个编译小文件却慢得多。然而这个解决方案并不能在像VMDE这
//   样的合作项目中进行，因为它会让很多已建立的条件反射、编译器、IDE日狗。
//   单一头文件也是一个可行的方案。它保留了头文件，但只有一个——这意味着所有
//   你需要编辑的头都在固定的位置，给身体和手带来了放松的空间，同时也保证编辑
//   器的标签栏（如果有的话）不会因为头文件而缩小为原来的一半。
//   当然，还有一个解决办法就是只用一个.c文件。结果：从此这个项目就没落了。
//   综上所述，我选择狗带，砍了其他所有头。
//=============================================================================

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <math.h>
#include <ctime>
#include <thread>
#include <cstdarg>
#include <cassert>
#include <execinfo.h>
#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <lodepng.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <portaudio.h>
#include <vorbis/vorbisfile.h>

#include <ASM76.hpp>

#ifndef _INCLUDE_GLOBAL_H
	#define _INCLUDE_GLOBAL_H
	using namespace std;
	//-------------------------------------------------------------------------
	// ● PY Deal For ＭICR0$○F┬ Ｗindoges (ᴚ)
	//Becuase it;s Windoges,I jsut dno't want to use CORERCT ENGRISh &忠闻吔屎炉此
	//-------------------------------------------------------------------------
	#ifdef __MINGW32__
		#define EXPORTED extern "C" __declspec(dllexport)
	#else
		#define EXPORTED extern "C"
	#endif
	#ifdef __CYGWIN__
		#warning This will not work in Cygwin. Try at your own risk.
	#endif
	//-------------------------------------------------------------------------
	// ● 定义宏魔法
	//-------------------------------------------------------------------------
	// NDEBUG - 给assert用的，虽然好像这项目里没人用assert
	// 这个DEBUG在make的时候加（make debug）。
	#ifdef DEBUG
		#define NDEBUG
	#else
		#undef NDEBUG
	#endif
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
	// XE - 释放内存黑魔法
	// “X掉Exceptions”！
	//     void* m;
	//     m = malloc(1); xe(free, m);
	//     m = (void*) 0; xe(free, m);
	//     m = new float; xe(delete, m);
	//     m = new tm[4]; xe(delete[], m);
	#define XE(method, pointer) do { \
		if (pointer) { \
			method(pointer); \
			(pointer) = NULL; \
		} \
	} while (false)
	#define SuperXE(method, pointer) do { \
		switch ((uintptr_t) pointer) { \
		case 0xbaadf00d: \
			(pointer) = NULL; \
			[[fallthrough]]; \
		case 0: \
			break; \
		default: \
			method(pointer); \
			(pointer) = NULL; \
		} \
	} while (false)
	// VMDE_Dispose - 一键销毁宏魔法
	#define VMDE_Dispose(method, object) do { \
		if (object) { \
			method(object); \
			(object) = NULL; \
		} \
	} while (false)
	// DEBUG_ENVIRONMENT - 调试环境
	// 这个字符串会显示在log函数输出的开头。
	#define DEBUG_ENVIRONMENT "VMDE"
	// log - Util::log_internal的方便缩写，可以直接得到当前函数名
	//     log("%p", log);
	#define log(...) Util::log_internal(DEBUG_ENVIRONMENT, __func__, __VA_ARGS__)
	// mark - 逐行打log的偷懒大法
	#define mark log("Mark on line %d of %s", __LINE__, __FILE__)
	// error_internal
	#define error_internal(extra, ...) do { \
		log(__VA_ARGS__); \
		extra \
		exit(1); \
	} while (false)
	// error - 抛出错误并终止程序
	#define error(...) error_internal(, __VA_ARGS__)
	// errorp - error + perror
	#define errorp(...) error_internal(perror("perror()");, __VA_ARGS__)
	// check_gl_error - 通过在代码中大量散布这个宏，可以达到调试器一般的效果（笑）
	#define check_gl_error Util::check_gl_error_internal(__FILE__, __LINE__)
	//-------------------------------------------------------------------------
	// ● util.cpp
	//   这个Util的意义已经远超utility。
	//-------------------------------------------------------------------------
	namespace Util {
		extern FILE* log_file;
		void init();
		void terminate();
		void log_internal(const char*, const char*, const char*, ...)
			__attribute__((format(printf, 3, 4)));
		char* read_file(const char* filename);
		void check_gl_error_internal(const char *file, unsigned line);
	}
	//-------------------------------------------------------------------------
	// ● 子文件夹中的头文件
	//-------------------------------------------------------------------------
	#include "VLib/V.hpp"
	#include "Audio/audio.hpp"
	//-------------------------------------------------------------------------
	// ● init.cpp
	//-------------------------------------------------------------------------
	EXPORTED void init_engine(int w, int h, const char* title);
	void setup_viewport();
	void init_vmde(int w, int h);
	//-------------------------------------------------------------------------
	// ● terminate.cpp
	//-------------------------------------------------------------------------
	EXPORTED void terminate_engine();
	//-------------------------------------------------------------------------
	// ● main.cpp
	//-------------------------------------------------------------------------
	void glfw_error_callback(int error, const char* description);
	void main_draw_start();
	void main_draw_end();
	void main_set_brightness(float b);
	//-------------------------------------------------------------------------
	// ● RenderObject.cpp
	//-------------------------------------------------------------------------
	class Object {
	public:
		virtual ~Object();
	};

	class RenderObject : public Object {
	public:
		virtual void render() = 0;
	};
	//-------------------------------------------------------------------------
	// ● resources.cpp
	//-------------------------------------------------------------------------
	namespace Res {
		struct TextureParameters {
			GLuint WRAP_MODE_S, WRAP_MODE_T;
			GLuint MIN_FILTER, MAG_FILTER;
			GLuint MIPMAP_LEVEL;
			GLuint PixelFormat, PixelType;
		};

		struct CubeMapFiles {
			const char *A, *B, *C, *D, *E, *F;
		};

		extern struct TextureParameters DefaultTextureParameters;
		extern struct TextureParameters LinearTextureParameters;

		class Texture : public Object {
		public:
			GLuint texture, index;
			int width, height;
			struct TextureParameters* parameter;

			static unsigned char* loadpng_bytes(const char* file, GLint* w, GLint* h);

			Texture();
			Texture(const char* file, struct TextureParameters* p);

			~Texture();
		};

		class CubeMap : public Texture {
		public:
			CubeMap(struct CubeMapFiles files, struct TextureParameters* p);
		};
	}
	//-------------------------------------------------------------------------
	// ● shaders.cpp
	//-------------------------------------------------------------------------
	class Shaders : public Object {
	public:
		#define SHADERS_SLOT_COUNT 16
		GLuint shaders[SHADERS_SLOT_COUNT];
		size_t shader_count = 0;
		GLuint program;

		GLuint UBO_matrix;
		glm::mat4 mat[3];

	public:
		Shaders();
		void add_string(GLenum type, const GLchar* source);
		void add_file(GLenum type, const char* filename);
		void link_program();
		void use();

		void set_float(const char* identifier, GLfloat value);
		void set_int(const char* identifier, GLint value);
		void set_vec2(const char* identifier, glm::vec2 value);
		void set_vec3(const char* identifier, glm::vec3 value);
		void set_vec4(const char* identifier, glm::vec4 value);
		void set_texture(const char* identifier, Res::Texture* tex, GLuint index);
		void set_texture_cube(const char* identifier, Res::CubeMap* tex, GLuint index);

		bool set_uniform_block(const char* identifier, GLuint* UBO, size_t size, GLuint type);
		void update_uniform_block(GLuint UBO, size_t size, GLuint* buf);

		void ProjectionView(glm::mat4 projection, glm::mat4 view);

		~Shaders();

	private:
		static void check_shader_compilation(
			GLuint shader,
			const char* msg = "shader compile error"
		);
		static void check_linkage(
			GLuint program,
			const char* msg = "link error"
		);
	};
	extern Shaders* _shaders_in_use;
	//-------------------------------------------------------------------------
	// ● VMDE
	//-------------------------------------------------------------------------
	struct VMDEState {
		bool frozen;
		float brightness;
	};

	enum GL_VER {
		GL_33,
		GL_43,
		VULKAN // Not used
	};

	struct VMDE {
		VMDEState state;
		long frame_count;
		long millisecond;
		int width, height;
		int fps;
		double frame_time;
		bool done;

		int gl_ver;
	};

	// VMDE操控的全局变量
	extern struct VMDE* VMDE;
	extern GLFWwindow* window;

	//-------------------------------------------------------------------------
	// ● State Control
	//-------------------------------------------------------------------------
	//   在OpenGL状态机外层再加一个状态管理机制，减少不必要的状态切换和不必要的draw call
	//   这样的一个状态机设计也可以更好地隔离API，为以后Vulkan做准备
	class VMStateControl {
		static struct StateMachineStruct {
			GLuint VERTEX_ARRAY;
			GLuint ELEMENT_ARRAY_BUFFER;
			GLuint ARRAY_BUFFER;
			GLuint TEXTURE_2D;
			GLuint TEXTURE_CUBE_MAP;
			GLuint UNIFORM_BUFFER;
			GLuint Shader_Program;
			bool TextureActivated[32]; // 32 for GL 3.X+, should be enough

			bool DEPTH_TEST;
			bool STENCIL_TEST;
			bool CULL_FACE;
			bool BLEND;
			GLuint PolygonMode;
		} StateMachine;

	public:
		static void ChangeVertexArray (GLuint index);
		static void ChangeElementArrayBuffer (GLuint index);
		static void ChangeArrayBuffer (GLuint index);
		static void ChangeTexture2D (GLuint index);
		static void ChangeTextureCubeMap (GLuint index);
		static void ChangeUniformBuffer (GLuint index);
		static void ChangeShaderProgram (GLuint index);

		static void enable_cullface();
		static void disable_cullface();
		static void enable_depth_test();
		static void disable_depth_test();
		static void enable_stencil_test();
		static void disable_stencil_test();
		static void enable_blend();
		static void disable_blend();
		static void render_mode_wireframe();
		static void render_mode_fill();

		static void init_graphics_state ();
	};
	#define VMSC VMStateControl // 少写几个字

	//-------------------------------------------------------------------------
	// ● GDrawable
	//-------------------------------------------------------------------------
	struct Vertex {
		glm::vec3 world_position;
		glm::vec4 color;
		glm::vec2 st;
		glm::vec3 normal;
	};

	struct VBO_Entry_Descriptor {
		size_t entry_length;
		int attribute_count;
		GLuint* attribute_size;
		GLuint* attribute_type;
		// Instanced data
		bool is_instanced;
		GLuint* divisors;
	};

	extern struct VBO_Entry_Descriptor default_Vertex;
	extern struct VBO_Entry_Descriptor default_MBO;

	class GDrawable : public RenderObject {
	public:
		struct VBO_Entry_Descriptor *vbo, *mbo;

		struct Data {
			GLuint* vertices;
			int vtx_c;
			GLuint* indices;
			int ind_c;
			GLuint* mat;
			int mat_c;
			GLuint MBO;
			GLuint VAO;
			GLuint VBO;
			GLuint EBO;
			glm::mat4 model;
		} data;

		void renderOnce();
		void render();
		void fbind();
		void update();
		void update_instance();
		void update_instance_alien_size();

		static inline void close_draw_node() { VMStateControl::ChangeVertexArray(0); }

		~GDrawable();

		GDrawable(struct VBO_Entry_Descriptor* vbo, struct VBO_Entry_Descriptor* mbo);
	};

	//-------------------------------------------------------------------------
	// ● Text
	//-------------------------------------------------------------------------
	class TextRenderer : public RenderObject {
	private:
		GDrawable* obj;
		Res::Texture tex = Res::Texture("../Media/Font.png", &Res::LinearTextureParameters);;
		Shaders texshader;

	public:
		enum TextDecorationType {
			NONE,
			SHADOW,
			OUTLINE,
		};
		TextRenderer();
		void BakeText(
			const char* text,
			float width, float height,
			TextDecorationType decoration
		);
		void render();
		void instanceRenderText(
			const char* text,
			glm::mat4 projection, glm::mat4 view, glm::mat4 transform,
			float width, float height, TextDecorationType decoration
		);
	};

	//-------------------------------------------------------------------------
	// ● RenderBuffer
	//-------------------------------------------------------------------------
	class RenderBuffer : public Object {
	public:
		GLuint framebuffer;
		GLuint rbo;
		Res::Texture** texture_buffer;
		int mrtcount;

		//void resize(int w, int h);

		void bind();
		void unbind();

		static void clearBuffer(glm::vec4 c, bool color, bool depth, bool stencil);

		RenderBuffer(int w, int h, int mrt, const GLuint* type);
		void set_draw_buffers();
		~RenderBuffer();
	};

	class PostProcessingManager : public Object {
	private:
		static GDrawable* QuadScreen;

	public:
		static void init();
		static void Blit2D();
	};

	//-------------------------------------------------------------------------
	// ● CommandList
	//-------------------------------------------------------------------------
	enum CLEnum {
		#define INT(i, x) CLEnum_##x = i,
		#include "CommandList.hpp"
	};

	class CmdList {
	private:
		static ASM76::BIOS* bios;
		ASM76::VM* vm;

	public:
		static void global_init();
		void call();

		CmdList(ASM76::Program p);
		~CmdList();
	};

#endif
