#include <stdio.h>
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
js_console_log_int(int64 param);

EM_PORT_API(int64)
Func(int64 param[], int len)
{
    js_console_log_int(len);
    for (int i = 0; i < len; i++)
    {
        int64 arg = param[i];
        js_console_log_int(arg);
    }
    js_console_log_int(9876543219999);
    FixedFloat a = FixedFloat(123456);
    return a.value;
}