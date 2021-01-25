//
// Created by Lai on 2021/1/20.
//

#ifndef WHITEBOARD_WHITEBOARDENGINE_H
#define WHITEBOARD_WHITEBOARDENGINE_H


#include <comm_value.h>
#include "egl/GlLooper.h"
#include "shader/BgShader.h"
#include "shader/ResultShader.h"
#include "shader/PaintShader.h"

class WhiteboardEngine {
private:
    GlLooper *glLooper;
    BgShader *bgShader;
    ResultShader *resultShader;
    PaintShader *paintShader;

    void requestRendering(std::function<void()> requestFunction, bool isFresh);

public:
//初始化
    WhiteboardEngine(jobject surface, JNIEnv *env) : glLooper(new GlLooper(surface, env)),
                                                     bgShader(nullptr), resultShader(nullptr),
                                                     paintShader(nullptr) {}

    ~WhiteboardEngine();


    void init(int width,
              int height, ImageInfo *brushImageInfo);

    void glDrawPoints(float *points, int vertexCount);

    void glClearColor();

    void glDisplay();

    void onDestroy();
};


#endif //WHITEBOARD_WHITEBOARDENGINE_H
