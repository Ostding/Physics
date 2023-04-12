#ifndef __MY_PRINTF_H__
#define __MY_PRINTF_H__
#include <stdarg.h>
#include "types.h"

namespace physics
{
#ifdef __ANDROID__
#include <android/log.h>
#define LOG_TGA "Physics"
#define myPrintf(...) __android_log_print(ANDROID_LOG_INFO, LOG_TGA, __VA_ARGS__)

	static void printff(const char *pszCap, const ffloat &f)
	{
		char psz[256] = {0};
		sprintf(psz, "%s => %s \n", pszCap, tostring(f));
		__android_log_print(ANDROID_LOG_INFO, LOG_TGA, "%s", psz);
	}
#else

	static void myPrintf(const char *pszFormat, ...)
	{
		va_list args;
		va_start(args, pszFormat);
		vprintf(pszFormat, args);
		va_end(args);
	}

	static void printff(const char *pszCap, const ffloat &f)
	{
		printf("%s => %s \n", pszCap, tostring(f));
	}
#endif

}

#endif