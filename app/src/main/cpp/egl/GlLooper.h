//
// Created by Lai on 2020/12/18.
//

#ifndef OPENGLDEMO_GLLOOPER_H
#define OPENGLDEMO_GLLOOPER_H


#include "Looper.h"
#include "gl_render_loop.h"


class GlLooper : public Looper {

//OpenGL渲染状态
public:
    enum STATE {
        NO_SURFACE, //没有有效的surface
        ON_CREATE_SURFACE, //持有一个为初始化的新的surface
        RENDERING, //初始化完毕，可以开始渲染
        NO_RENDERING,
        SURFACE_DESTROY, //surface销毁
        STOP //停止绘制
    };


public:
    GlLooper(jobject surface, JNIEnv *env);

    virtual ~GlLooper();

    virtual void handleMessage(LooperMessage *msg);

    void init();

    void onDestroy();

private:
    GlLoopRender *glLoopRender = nullptr;
    STATE m_state = NO_SURFACE;
    bool isSwapBuffers = true;
public:
    void setIsSwapBuffers(bool isSwapBuffers);
};


#endif //OPENGLDEMO_GLLOOPER_H
