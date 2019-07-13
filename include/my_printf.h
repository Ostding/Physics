#ifndef __MY_PRINTF_H__
#define __MY_PRINTF_H__
#include <stdarg.h>

#ifdef __ANDROID__
	#include <android/log.h>
	#define LOG_TGA "Physics"
	#define myPrintf(...) __android_log_print(ANDROID_LOG_INFO, LOG_TGA, __VA_ARGS__)

#else
	static void myPrintf(const char *pszFormat, ...)
	{
	  va_list args;
	  va_start(args, pszFormat);
	  vprintf(pszFormat, args);
	  va_end(args);
	}
#endif

#endif