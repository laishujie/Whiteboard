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
    /* BgShader *testShader;*/
    ResultShader *resultShader;
    PaintShader *paintShader;
    JavaVM *mJavaVM;
    jobject initCallBack;
    jobject saveCallBack;

    int mNeedDetach = 0;

    void requestRendering(std::function<void()> requestFunction, bool isFresh);
    void noRequestRendering(std::function<void()> requestFunction, bool isFresh);

    bool isDraw();

public:
//初始化
    WhiteboardEngine(jobject surface, JNIEnv *env) : glLooper(new GlLooper(surface, env)),
                                                     bgShader(nullptr)/*, testShader(nullptr)*/,
                                                     resultShader(nullptr),
                                                     paintShader(nullptr), mJavaVM(nullptr),
                                                     initCallBack(nullptr), saveCallBack(nullptr) {}

    ~WhiteboardEngine();

    void init(int width,
              int height, ImageInfo *brushImageInfo, JNIEnv *env, jobject callBack);

    void glDrawPoints(float *points, int vertexCount, float textureRotate);

    void glClearColor();

    void glResultMatrix(float *f);

    void glResult(float r, float restR, float dx, float dy, float sc);

    void glTranslate(float dx, float dy);

    void glScale(float s);

    void glRotate(float s);

    void glSetPaintTexture(ImageInfo *brushImageInfo,
                           float brushWidth,
                           bool isTextureRotate,
                           BrushInfo::OutType outType);

    void setPaintColor(float A,
                       float R,
                       float G,
                       float B);

    void glDrawData(float *points,
                    int vertexCount, ImageInfo *brushImageInfo,
                    float brushWidth, BrushInfo::OutType outType, bool isTextureRotate,
                    bool isClear, bool isDisplay,
                    float A,
                    float R,
                    float G,
                    float B);

    void glDisplay();

    void glClearPaint();

    void glSave(JNIEnv *env, char *path, jobject callBack);

    void voidCallBack(jobject voidCallBack);

    void onDestroy();


};


#endif //WHITEBOARD_WHITEBOARDENGINE_H
