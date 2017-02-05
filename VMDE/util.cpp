//=============================================================================
// ■ util.cpp
//-----------------------------------------------------------------------------
//   提供实用函数：打印日志。
//=============================================================================

#include "global.hpp"

namespace Util {
	//-------------------------------------------------------------------------
	// ● 常量
	//-------------------------------------------------------------------------
	const float PIf = acos(-1.0f);
	const double PI = acos(-1.0); // no suffix
	const long double PIl = acos(-1.0l);
	//-------------------------------------------------------------------------
	// ● 变量
	//-------------------------------------------------------------------------
	FILE* log_file = NULL;
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
		printf(
			"[%02d:%02d:%02d][%s][%s] ",
			t->tm_hour, t->tm_min, t->tm_sec,
			environment, function_name
		);
		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
		if (log_file) {
			// do it again rudely
			va_start(ap, format);
			fprintf(log_file,
				"[%02d:%02d:%02d][%s][%s] ",
				t->tm_hour, t->tm_min, t->tm_sec,
				environment, function_name
			);
			vfprintf(log_file, format, ap);
			fputc('\n', log_file);
			va_end(ap);
		}
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
		r[size < 1024 ? size : 1023] = 0;
		fclose(f);
		return r;
	}
}
