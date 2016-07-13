//=============================================================================
// ■ util.cpp
//-----------------------------------------------------------------------------
//   提供实用函数：打印日志。
//=============================================================================

#include "global.hpp"

namespace Util {
	//-------------------------------------------------------------------------
	// ● 输出调试信息
	//   * 如果需要报错，请使用Ruby提供的rb_raise函数，例如
	//     rb_raise(rb_eRuntimeError, "mystery = %p", time);。
	//    function_name : 如无意外，应当传入__func__。
	//    format, ...   : 与printf风格一致。
	//   凭借宏的力量（global.hpp），这个函数应当直接使用log(format, ...)调用。
	//-------------------------------------------------------------------------
	void log_internal(const char* function_name, const char* format, ...) {
		time_t time_t_time = time(NULL);
		// Warning: This function may not be thread-safe.
		tm* t = localtime(&time_t_time);
		va_list ap;
		va_start(ap, format);
		printf(
			"[%02d:%02d:%02d][%s][%s] ",
			t->tm_hour, t->tm_min, t->tm_sec,
			DEBUG_ENVIRONMENT, function_name
		);
		vprintf(format, ap);
		putchar('\n');
		va_end(ap);
	}
}
