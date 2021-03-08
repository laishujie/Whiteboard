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

void WhiteboardEngine::noRequestRendering(std::function<void()> requestFunction, bool isFresh) {
    glLooper->postMessage(glLooper->NO_RENDERING, requestFunction, isFresh);
}

bool WhiteboardEngine::isDraw() {
    return paintShader == nullptr || bgShader == nullptr || resultShader == nullptr;
}


void WhiteboardEngine::init(int w, int h, ImageInfo *brush, JNIEnv *env, jobject callBack) {

    //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
    if (mJavaVM == nullptr)
        env->GetJavaVM(&mJavaVM);
    if (initCallBack != nullptr)
        initCallBack = nullptr;
    //生成一个全局的callBack
    initCallBack = env->NewGlobalRef(callBack);

    //在Gl线程渲染
    requestRendering([=] {
        bgShader = new BgShader();
        bgShader->Init();
        bgShader->OnSurfaceChanged(w, h);
        bgShader->draw();

        paintShader = new PaintShader();
        paintShader->Init();
        paintShader->glSetBrush(brush, 15.0f, false, BrushInfo::DRAW);
        paintShader->OnSurfaceChanged(w, h);
        paintShader->draw();

        resultShader = new ResultShader();
        resultShader->Init();
        resultShader->OnSurfaceChanged(w, h);

        glDisplay();

        voidCallBack(initCallBack);


    }, false);
}


void WhiteboardEngine::voidCallBack(jobject voidCallBack) {
    JNIEnv *mEnvs = nullptr;
    int getEnvStat = mJavaVM->GetEnv((void **) &mEnvs, JNI_VERSION_1_6);
    //获取当前native线程是否有没有被附加到jvm环境中
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (mJavaVM->AttachCurrentThread(&mEnvs, NULL) != 0) {
            return;
        }
        mNeedDetach = JNI_TRUE;
    }

    jclass javaClass = mEnvs->GetObjectClass(voidCallBack);

    if (javaClass == 0) {
        LOGCATE("Unable to find javaClass == 0")
        return;
    }

    //获取要回调的方法ID
    jmethodID javaCallbackId = mEnvs->GetMethodID(javaClass, "voidCallBack", "()V");
    if (javaCallbackId == NULL) {
        LOGCATE("Unable to find method:test")
        return;
    }
    mEnvs->CallVoidMethod(voidCallBack, javaCallbackId);
    if (mEnvs != nullptr && voidCallBack != nullptr)
        mEnvs->DeleteGlobalRef(voidCallBack);

    if (mNeedDetach) {
        mJavaVM->DetachCurrentThread();
    }

    mNeedDetach = 0;
    mJavaVM = nullptr;
    mEnvs = nullptr;
}

void WhiteboardEngine::glDrawPoints(float *points, int vertexCount, float textureRotate) {
    if (isDraw()) return;

    requestRendering([=] {
        paintShader->glUpdatePoints(points, vertexCount, textureRotate, false);
        glDisplay();
    }, false);
}


void WhiteboardEngine::glSetPaintTexture(ImageInfo *brushImageInfo, float brushWidth,
                                         bool isTextureRotate,
                                         BrushInfo::OutType outType) {
    if (isDraw()) return;

    if (brushImageInfo == nullptr) {
        paintShader->glSetBrush(brushImageInfo, brushWidth, isTextureRotate, outType);
    } else {
        //需要在gl线程渲染
        noRequestRendering([=] {
            paintShader->glSetBrush(brushImageInfo, brushWidth, isTextureRotate, outType);
            glDisplay();
        }, false);
    }
}

void WhiteboardEngine::glDrawData(float *points, int vertexCount, ImageInfo *brushImageInfo,
                                  float brushWidth, BrushInfo::OutType outType,
                                  bool isTextureRotate, bool isClear,
                                  bool isDisplay, float A,
                                  float R,
                                  float G,
                                  float B) {

    requestRendering([=] {
        if (isDraw()) return;
        paintShader->glSetBrush(brushImageInfo, brushWidth, isTextureRotate, outType);
        paintShader->setPaintColor(A, R, G, B);
        paintShader->glUpdatePoints(points, vertexCount, 0.0f, isClear);
        glLooper->setIsSwapBuffers(isDisplay);
        if (isDisplay)
            glDisplay();
    }, false);

}

void WhiteboardEngine::setPaintColor(float A, float R, float G, float B) {
    if (isDraw()) return;
    paintShader->setPaintColor(A, R, G, B);
}


void WhiteboardEngine::onDestroy() {
    noRequestRendering([=] {
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
    }, true);

    if (glLooper != nullptr) {
        glLooper->onDestroy();
        delete glLooper;
        glLooper = nullptr;
    }
    if (saveCallBack != nullptr)
        saveCallBack = nullptr;
    if (initCallBack != nullptr)
        initCallBack = nullptr;
}

void WhiteboardEngine::glResultMatrix(float *f) {
    if (isDraw()) return;
    if (f != nullptr) {
        requestRendering([=] {
            resultShader->setMMatrix(f);
            glDisplay();
        }, true);
    }
}

void WhiteboardEngine::glResult(float r, float restR, float dx, float dy, float sc) {
    if (isDraw()) return;
    requestRendering([=] {
        resultShader->setTransformation(r, restR, dx, dy, sc);
        glDisplay();
    }, true);
}

void WhiteboardEngine::glTranslate(float dx, float dy) {
    if (isDraw()) return;
    requestRendering([=] {
        resultShader->setTransformation(0.0f, 0.0f, dx, dy, 0.0f);
        glDisplay();
    }, true);
}

void WhiteboardEngine::glScale(float s) {
    if (isDraw()) return;
    requestRendering([=] {
        resultShader->setTransformation(0.0f, 0.0f, 0.0f, 0.0f, s);
        glDisplay();
    }, true);
}


void WhiteboardEngine::glRotate(float r) {
    if (isDraw()) return;
    requestRendering([=] {
        resultShader->setTransformation(r, 0.0f, 0.0f, 0.0f, 0.0f);
        glDisplay();
    }, true);
}

void WhiteboardEngine::glDisplay() {
    if (isDraw()) return;

    resultShader->mergeTextureDisplay(bgShader->bgTextureId, paintShader->paintTextureId);
}

void WhiteboardEngine::glClearPaint() {
    if (isDraw()) return;
    requestRendering([=] {
        paintShader->glClearPaint();
    }, true);
}


void WhiteboardEngine::glClearColor() {
    requestRendering([=] {
        paintShader->glClearPaint();
        bgShader->glClearBg();
        bgShader->draw();
        glDisplay();
    }, true);
}

void WhiteboardEngine::glSave(JNIEnv *env, char *path, jobject callBack) {
    //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
    if (mJavaVM == nullptr)
        env->GetJavaVM(&mJavaVM);

    if (saveCallBack != nullptr)
        saveCallBack = nullptr;
    //生成一个全局的callBack
    saveCallBack = env->NewGlobalRef(callBack);

    noRequestRendering([=] {
        resultShader->save(path);
        free(path);
        voidCallBack(saveCallBack);
    }, false);
}






