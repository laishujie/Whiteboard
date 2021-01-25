//
// Created by feibiao.ma on 2020/9/16.
//

#ifndef WHEAT_EGL_CORE_H
#define WHEAT_EGL_CORE_H

#include <EGL/egl.h>
#include <EGL/eglext.h>

class EglCore {
private:
    const char *TAG = "EglCore";
    //EGL显示窗口
    EGLDisplay m_egl_dsp = EGL_NO_DISPLAY;
    //EGL上下文
    EGLContext m_egl_context = EGL_NO_CONTEXT;
    //EGL配置
    EGLConfig m_egl_config;

    EGLConfig GetEGLConfig();

public:
    EglCore();

    ~EglCore();

    bool Init(EGLContext share_ctx);

    //根据比例窗口创建显示表面
    EGLSurface CreateWindSurface(ANativeWindow *window);

    EGLSurface CreateOffScreenSurface(int width, int height);

    //将OpenGL上下文与当前线程绑定
    void MakeCurrent(EGLSurface egl_surface);

    //将缓存数据交换至前台显示
    void SwapBuffer(EGLSurface egl_surface);

    //释放显示
    void DestroySurface(EGLSurface egl_surface);

    //释放EGL
    void Release();
};


#endif //WHEAT_EGL_CORE_H
