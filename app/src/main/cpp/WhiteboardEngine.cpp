//
// Created by Lai on 2021/1/20.
//

#include <logUtil.h>
#include "WhiteboardEngine.h"

WhiteboardEngine::~WhiteboardEngine() {
    onDestroy();
}

void WhiteboardEngine::requestRendering(std::function<void()> funcType, bool isFresh) {
    glLooper->postMessage(glLooper->RENDERING, funcType, isFresh);
}

void WhiteboardEngine::init(int w, int h, ImageInfo *brush) {
    if (bgShader != nullptr) {
        delete bgShader;
        bgShader = nullptr;
    }

    //在Gl线程渲染
    requestRendering([=] {
        bgShader = new BgShader();
        bgShader->Init();
        bgShader->OnSurfaceChanged(w, h);
        bgShader->draw();

        paintShader = new PaintShader();
        paintShader->Init();
        paintShader->glSetBrush(brush);
        paintShader->OnSurfaceChanged(w, h);
        paintShader->draw();

        resultShader = new ResultShader();
        resultShader->Init();
        resultShader->OnSurfaceChanged(w, h);
        glDisplay();
    }, false);
}

void WhiteboardEngine::glDrawPoints(float *points, int vertexCount) {
    if (paintShader == nullptr) return;

    requestRendering([=] {
        paintShader->glUpdatePoints(points, vertexCount);
        glDisplay();
    }, false);
}

void WhiteboardEngine::onDestroy() {
    requestRendering([=] {
        if (bgShader != nullptr) {
            delete bgShader;
            bgShader = nullptr;
        }
        if (paintShader != nullptr) {
            delete paintShader;
            paintShader = nullptr;
        }
        if (resultShader != nullptr) {
            delete resultShader;
            resultShader = nullptr;
        }
    }, false);

    if (glLooper != nullptr) {
        glLooper->onDestroy();
        delete glLooper;
        glLooper = nullptr;
    }
}

void WhiteboardEngine::glDisplay() {
    if (resultShader == nullptr || bgShader == nullptr || paintShader == nullptr) return;
    resultShader->mergeTexture(bgShader->frameBuffer, paintShader->paintTextureId);
    resultShader->draw(bgShader->bgTextureId, false);
}

void WhiteboardEngine::glClearColor() {
    requestRendering([=] {
        paintShader->glClearPaint();
        bgShader->glClearBg();
        bgShader->draw();
        glDisplay();
    }, true);
}






