#include <jni.h>
#include <string>
#include "thread_model.h"

void StartThread() {
    //IDbg::SetThreadName("main");

    IDbg::ThreadModel th{ "demo_thread" };
    th.PushTask([] {
        printf("in thread model");
        //std::cout << "in thread model " << std::this_thread::get_id() << std::endl;
    });
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_idbg_1demo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    StartThread();
    return env->NewStringUTF(hello.c_str());
}
