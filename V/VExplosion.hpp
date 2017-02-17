//=============================================================================
// ■ VExplosion.hpp
//-----------------------------------------------------------------------------
//   可以安全地在用户级代码中调用并获得SIGSEGV或其它异常，并在退出程序时装逼。
//=============================================================================

#ifdef __cplusplus
	#include <cstddef>
	#include <cstdlib>
	#include <csignal>
	#include <cstring>
	#include <cmath>
#else
	#include <stddef.h>
	#include <stdlib.h>
	#include <signal.h>
	#include <string.h>
	#include <math.h>
#endif

// EXPLOSION - 使用哪一个实现？
#define EXPLOSION EXPLOSION_ASSIGNMENT

// EXPLOSION_ASSIGNMENT - 赋值实现
#define EXPLOSION_ASSIGNMENT (*((float*) NULL) = INFINITY)

// EXPLOSION_ASSIGNMENT_SIMPLE - 简易赋值实现
#define EXPLOSION_ASSIGNMENT_SIMPLE (*((int*) 0) = 0)

// EXPLOSION_MEMORY - 标准C库内存操作实现
#define EXPLOSION_MEMORY (memset(NULL, 233, 1))

// EXPLOSION_SIGNAL - 标准C库信号实现
#define EXPLOSION_SIGNAL (raise(SIGSEGV), abort())

// EXPLOSION_CONSTSTR - 只读字符串实现
#define EXPLOSION_CONSTSTR (*((char*) "") = 0)

// EXPLOSION_ASM - Pentium Pro指令实现
// * 这个实现不会导致SIGSEGV。
#ifdef __GNUC__
	#define EXPLOSION_ASM __asm__("UD2")
#endif

// EXPLOSION_CALL - call 0的C版本
#define EXPLOSION_CALL ((void (*)()) 0)()
