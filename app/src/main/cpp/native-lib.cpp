#include <jni.h>
#include <logUtil.h>
#include "WhiteboardEngine.h"
#include <android/bitmap.h>
#include <malloc.h>
#include <string.h>


WhiteboardEngine *whiteboardEngine = nullptr;

extern "C" JNIEXPORT jstring JNICALL
Java_com_lai_whiteboard_ShaderNative_stringFromJNI(
        JNIEnv *env,
        jclass clazz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_lai_whiteboard_ShaderNative_init(JNIEnv *env, jclass clazz, jint width,
                                          jint height, jobject surface, jobject bitmap) {
    if (whiteboardEngine != nullptr) {
        delete whiteboardEngine;
        whiteboardEngine = nullptr;
    }

    AndroidBitmapInfo info; // create a AndroidBitmapInfo
    int result;
    // 获取图片信息
    result = AndroidBitmap_getInfo(env, bitmap, &info);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Player", "AndroidBitmap_getInfo failed, result: %d", result);
        return;
    }
    // 获取像素信息
    unsigned char *data;
    result = AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&data));
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Player", "AndroidBitmap_lockPixels failed, result: %d", result);
        return;
    }
    size_t count = info.stride * info.height;

    unsigned char *resultData = (unsigned char *) malloc(count * sizeof(unsigned char));;
    memcpy(resultData, data, count);

    // 像素信息不再使用后需要解除锁定
    result = AndroidBitmap_unlockPixels(env, bitmap);
    if (result != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Player", "AndroidBitmap_unlockPixels failed, result: %d", result);
    }

    ImageInfo *brushPoints = new ImageInfo(info.width, info.height, resultData);
    //初始化EGL
    whiteboardEngine = new WhiteboardEngine(surface, env);
    whiteboardEngine->init(width, height, brushPoints);
    //初始化绘制默认
}

extern "C" JNIEXPORT void JNICALL
Java_com_lai_whiteboard_ShaderNative_glDrawPaint(JNIEnv *env, jclass instance, jfloatArray point,
                                                 jint verTextSize) {
    jfloat *pDouble = env->GetFloatArrayElements(point, NULL);

    if (whiteboardEngine == nullptr) return;
    whiteboardEngine->glDrawPoints(pDouble, verTextSize);
}

extern "C" JNIEXPORT void JNICALL
Java_com_lai_whiteboard_ShaderNative_onDestroy(JNIEnv *env, jclass instance){
    if (whiteboardEngine != nullptr) {
        delete whiteboardEngine;
        whiteboardEngine = nullptr;
    }
    //whiteboardEngine->onDestroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_lai_whiteboard_ShaderNative_glClearPaintColor(JNIEnv *env, jclass instance){
    if (whiteboardEngine != nullptr) {
        whiteboardEngine->glClearColor();
    }
}

