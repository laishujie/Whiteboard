//
// Created by feibiao.ma on 2020/9/16.
//

#ifndef WHEAT_OPENGL_RENDER_H
#define WHEAT_OPENGL_RENDER_H


#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <memory>
#include <thread>
#include "egl_surface.h"

class GlLoopRender {
private:
    const char *TAG = "GlLoopRender";
    //Surface引用，必须要使用引用，否则无法在线程中操作
    jobject m_surface_ref = nullptr;
    //本地屏幕
    ANativeWindow *m_native_window = nullptr;
    //EGL显示表面
    EglSurface *m_egl_surface = nullptr;
   // BgShader *bgDemo = nullptr;
    int m_window_width = 0;
    int m_window_height = 0;
    //StickerGroup *stickerGroup = nullptr;

public:
    GlLoopRender() {
    }

    bool InitEGL();

    void InitDspWindow(JNIEnv *env);

    // 创建/销毁 Surface
    void CreateSurface();

    void DestroySurface();

    // 渲染方法
    void Render();

    void ReleaseSurface();

    void ReleaseWindow();

    ~GlLoopRender();

    void SetSurface(jobject surface, JNIEnv *m_env);

    //void addStickerTexture(TextureLayer layer);

    void Stop();

    //void SetBitmapRender(ImageRender *bitmapRender);


// 释放资源相关方法
    void ReleaseRender();

    //ImageRender *GetImageRender();
};


#endif //WHEAT_OPENGL_RENDER_H
