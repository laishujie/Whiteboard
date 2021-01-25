//
// Created by Lai on 2020/12/18.
//

#include <logUtil.h>
#include "GlLooper.h"


GlLooper::GlLooper(jobject surface, JNIEnv *env) {
    glLoopRender = new GlLoopRender();
    glLoopRender->SetSurface(surface, env);
    glLoopRender->InitDspWindow(env);
    glLoopRender->InitEGL();
    //关联furface
    postMessage(ON_CREATE_SURFACE);
    //LOGE("11111", "InitEGL")
}


void GlLooper::handleMessage(LooperMessage *msg) {
    LOGE("11111", "msg->what %d ", msg->what)
    m_state = static_cast<STATE>(msg->what);

    switch (m_state) {
        //創建surface
        case ON_CREATE_SURFACE:
            LOGE("11111", "Loop Render ON_CREATE_SURFACE")
            if (!glLoopRender->InitEGL()) {
                return;
            }
            glLoopRender->CreateSurface();
            break;
        case RENDERING:
            LOGE("11111", "msg->funcType");
            if(msg->funcType!= nullptr){
               msg->funcType();
            }
            LOGE("11111", "Loop Render RENDERING");
            if(glLoopRender!= nullptr)
            glLoopRender->Render();
            break;
        case SURFACE_DESTROY:
            LOGE("11111", "Loop Render SURFACE_DESTROY")
            glLoopRender->DestroySurface();
            break;
        case STOP:
            LOGE("11111", "Loop Render STOP")
            //解除线程和jvm关联
            glLoopRender->ReleaseRender();
            return;
        case NO_SURFACE:
        default:
            break;
    }
}


void GlLooper::onDestroy() {
    postMessage(STOP);
    quit();
}
void GlLooper::init() {
    postMessage(ON_CREATE_SURFACE);
}

GlLooper::~GlLooper() {
    delete glLoopRender;
    glLoopRender= nullptr;
}