#include <stdio.h>
#include <stdint.h>
#define DISABLE_RENDER
#include "./../../../include/physics.h"
using namespace physics;

#ifndef EM_PORT_API
#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#else
#define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#endif
#else
#if defined(__cplusplus)
#define EM_PORT_API(rettype) extern "C" rettype
#else
#define EM_PORT_API(rettype) rettype
#endif
#endif
#endif

// int main()
// {
//     printf("hello world!");
//     return 0;
// }

EM_PORT_API(void)
js_console_log_int64(const char *psz, int64 param);

EM_PORT_API(void)
js_console_log_int(const char *psz, int param);

EM_PORT_API(int64)
Func(unsigned char *u8data, int dataLength)
{
    int len = dataLength / sizeof(int64);
    js_console_log_int("Func recv arg int64 count:", len);

    int byteLen = (sizeof(int64) / sizeof(unsigned char));
    for (int i = 0; i < len; i++)
    {
        int64 arg = *((int64 *)(u8data + i * byteLen));
        js_console_log_int64("      args:", arg);
    }
    js_console_log_int64("Log int64:", 9876543219999);
    FixedFloat a = FixedFloat(123456);
    return a.value;
}

EM_PORT_API(int64)
FuncAdd(int64 a, int64 b)
{
    return a + b;
}

EM_PORT_API(void)
Func1(int64 param, const char *pszStr)
{
    js_console_log_int64(pszStr, param);
}
