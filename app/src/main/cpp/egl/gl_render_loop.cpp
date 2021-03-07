//
// Created by feibiao.ma on 2020/9/16.
//

#include <unistd.h>
#include <GLES3/gl3.h>
#include <logUtil.h>
#include "gl_render_loop.h"
#include "egl_surface.h"


GlLoopRender::~GlLoopRender() {
    delete m_egl_surface;
    m_egl_surface = nullptr;
}

bool GlLoopRender::InitEGL() {
    if(m_egl_surface!= nullptr){
        delete m_egl_surface;
        m_egl_surface = nullptr;
    }
    m_egl_surface = new EglSurface();
    return m_egl_surface->Init();
}

void GlLoopRender::SetSurface(jobject surface, JNIEnv *m_env) {
    if (nullptr != surface) {
        m_surface_ref = m_env->NewGlobalRef(surface);
    } else {
        m_env->DeleteGlobalRef(m_surface_ref);
    }
}

void GlLoopRender::InitDspWindow(JNIEnv *env) {
    //LOGE("11111","m_surface_ref %p",m_surface_ref)

    if (m_surface_ref != nullptr) {
        // 初始化窗口
        m_native_window = ANativeWindow_fromSurface(env, m_surface_ref);

        // 绘制区域的宽高
        m_window_width = ANativeWindow_getWidth(m_native_window);
        m_window_height = ANativeWindow_getHeight(m_native_window);
        //设置宽高限制缓冲区中的像素数量
        ANativeWindow_setBuffersGeometry(m_native_window, m_window_width,
                                         m_window_height, WINDOW_FORMAT_RGBA_8888);
    }
}

void GlLoopRender::CreateSurface() {
    m_egl_surface->CreateEglSurface(m_native_window, m_window_width, m_window_height);
    glViewport(0, 0, m_window_width, m_window_height);
}



void GlLoopRender::Render() {
    m_egl_surface->SwapBuffers();
}

void GlLoopRender::Stop() {

}

void GlLoopRender::ReleaseRender() {
    Stop();
    ReleaseSurface();
    ReleaseWindow();
}

void GlLoopRender::DestroySurface() {
    m_egl_surface->DestroyEglSurface();
    ReleaseWindow();
}

void GlLoopRender::ReleaseSurface() {
    if (m_egl_surface != nullptr) {
        m_egl_surface->Release();
        delete m_egl_surface;
        m_egl_surface = nullptr;
    }
}

void GlLoopRender::ReleaseWindow() {
    if (m_native_window != nullptr) {
        ANativeWindow_release(m_native_window);
        m_native_window = nullptr;
    }
}





