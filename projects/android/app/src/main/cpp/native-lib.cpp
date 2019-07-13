#include <jni.h>
#include <string>
#include "test_fixed_float.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_physics_demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_physics_demo_MainActivity_testFixedFloat( JNIEnv *env,
        jobject)
{
    TestFixedFloat::doTest();
    TestFixedFloat::doTestSpeed();
    return env->NewStringUTF("Test complete, plz check log with adb logcat!!!");
}
