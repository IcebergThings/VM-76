//=============================================================================
// ■ util.cpp
//-----------------------------------------------------------------------------
//   提供实用函数：打印日志。
//=============================================================================

#include "global.hpp"

namespace Util {
	//-------------------------------------------------------------------------
	// ● 变量
	//-------------------------------------------------------------------------
	FILE* log_file = NULL;
	// log用（每条至多1K，超出部分将截断；每条5毛，括号内删除）
	#define TMP_LOG_LINE_SIZE ((size_t) 1024)
	static char tmp_log_line[TMP_LOG_LINE_SIZE];
	//-------------------------------------------------------------------------
	// ● 初始化
	//-------------------------------------------------------------------------
	void init() {
		log_file = fopen("output.log", "w");
	}
	//-------------------------------------------------------------------------
	// ● 结束处理
	//-------------------------------------------------------------------------
	void terminate() {
		fclose(log_file);
		log_file = NULL; // in case of calls to log() after termination
	}
	//-------------------------------------------------------------------------
	// ● 输出调试信息
	//   凭借宏的力量（global.hpp），这个函数应当直接使用log(format, ...)调用。
	//    environment   : 如无意外，应当传入DEBUG_ENVIRONMENT。
	//    function_name : 如无意外，应当传入__func__。
	//    format, ...   : 与printf风格一致。
	//-------------------------------------------------------------------------
	void log_internal(
		const char* environment,
		const char* function_name,
		const char* format,
		...
	) {
		time_t time_t_time = time(NULL);
		// Warning: This function may not be thread-safe.
		tm* t = localtime(&time_t_time);
		va_list ap;
		va_start(ap, format);
		int error = snprintf(
			tmp_log_line,
			TMP_LOG_LINE_SIZE,
			"[%02d:%02d:%02d][%s][%s] ",
			t->tm_hour, t->tm_min, t->tm_sec,
			environment, function_name
		);
		// printf都出错的话我还能干啥？
		unsigned int size = (unsigned int) error;
		// 整行就剩个消息头了还输出什么？
		if (size >= TMP_LOG_LINE_SIZE) {
			va_end(ap);
			return;
		}
		size += vsnprintf(
			tmp_log_line + size,
			TMP_LOG_LINE_SIZE - size,
			format,
			ap
		);
		// 这个就算了
		if (size >= TMP_LOG_LINE_SIZE) size = TMP_LOG_LINE_SIZE - 1;
		va_end(ap);
		puts(tmp_log_line);
		tmp_log_line[size] = '\n';
		tmp_log_line[size + 1] = 0;
		if (log_file) fputs(tmp_log_line, log_file);
	}
	//-------------------------------------------------------------------------
	// ● 一次性读入文件并加零（固定大小1KB，超出将截断）
	//   使用者需自行释放（free）它。
	//-------------------------------------------------------------------------
	char* read_file(const char* filename) {
		FILE* f = fopen(filename, "rb");
		if (!f) return NULL;
		char* r = (char*) malloc(1024);
		if (!r) return NULL;
		size_t size = fread(r, 1, 1024, f);
		if (size < 1024 ? size : 1023) {
			r[size] = 0;
		} else {
			r[1023] = 0;
			log("Warning: read_file has truncated file %s", filename);
		}
		fclose(f);
		return r;
	}
	//-----------------------------------------------------------------------------
	// ● 检查GL是否有错误
	//   是时候好好用用unsigned了！
	//   这个平时经常惹麻烦的家伙，在这里却什么也干不了。
	//   当然了，千万不要因为数据一定是非负的而采用无符号整数，
	//   要不然你会非常痛苦的。别问我是怎么知道的。
	//-----------------------------------------------------------------------------
	void check_gl_error_internal(const char* file, unsigned line) {
		GLenum error;
		int count = 0;
		while ((error = glGetError()) != GL_NO_ERROR) {
			count++;
			if (count == 1) log("GL error!");
			switch (error) {
			#define case(x) case x: printf("- %s\n", #x); break
			case(GL_INVALID_ENUM);
			case(GL_INVALID_VALUE);
			case(GL_INVALID_OPERATION);
			case(GL_INVALID_FRAMEBUFFER_OPERATION);
			case(GL_OUT_OF_MEMORY);
			#undef case
			default:
				printf("- %d\n", error);
				break;
			}
		}
		if (count) {
			printf(
				"These are all the errors from glGetError()"
				" that were thrown before the check in:\n\t%s:%d.\n",
				file, line
			);
			error("Terminating to arouse your attention.");
		}
	}
}
