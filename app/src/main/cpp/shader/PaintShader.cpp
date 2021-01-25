//
// Created by Lai on 2021/1/8.
//

#include <logUtil.h>
#include "PaintShader.h"


void PaintShader::Init() {
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position =  vPosition;              \n"
            "gl_PointSize = 40.0; \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "uniform sampler2D textureMap;\n"
            "void main()                                  \n"
            "{                                            \n"
            "vec4 mask = texture(textureMap, vec2(gl_PointCoord.x,gl_PointCoord.y));\n"
            // "   fragColor = 1.0 * vec4(1.0, 0.0, 0.0, 1.0) * mask;\n"
            "   fragColor = mask;\n"
            "}                                            \n";

    glProgram = new GLProgram(vShaderStr, fShaderStr);
    glvao = new GLVAO();


    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //像素偏移1
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    if (brushImageInfo != nullptr) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brushImageInfo->width, brushImageInfo->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     brushImageInfo->pixels);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    LOGE("11111", "PaintShader glProgram %d", glProgram->program)
}

void PaintShader::glSetBrush(ImageInfo *imageInfo) {
    if (brushImageInfo != nullptr) {
        delete brushImageInfo;
        brushImageInfo = nullptr;
    }
    brushImageInfo = imageInfo;

    glBindTexture(GL_TEXTURE_2D, textureId);
    if (brushImageInfo->pixels == nullptr) {
        LOGE("11111", "brushImageInfo->pixels== nullptr")
        return;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brushImageInfo->width, brushImageInfo->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE,
                 brushImageInfo->pixels);
}

PaintShader::~PaintShader() {
    onDestroy();
}

void PaintShader::onDestroy() {
    if (brushImageInfo != nullptr) {
        delete brushImageInfo;
        brushImageInfo = nullptr;
    }
    if(textureId>0){
        glDeleteTextures(1, &textureId);
    }
    if(frameBuffer!=0){
        glDeleteFramebuffers(1, &frameBuffer);
    }
    LOGE("11111", "onDestroy PaintShader")
}

void PaintShader::glUpdatePoints(float *points, int vertexCount) {
    LOGE("11111","vertexCount %d",vertexCount)
    paintFbo = glvao->setVertex2D(paintFbo, points, vertexCount, 0);
    defaultVertexCount = vertexCount;

    draw();
}

void PaintShader::glClearPaint() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PaintShader::draw() {

    //if (paintFbo == 0) return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    // 暂时切换为屏幕缓冲区
    //frameBuffer = 0;
    //glViewport(0,0,surfaceWidth,surfaceHeight);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //glViewport(0,0,surfaceWidth,surfaceHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    glProgram->useProgram();
    GLint textureIndex = glGetUniformLocation(glProgram->program, "textureMap");
    //激活纹理单元编号和纹理单元对应
    glActiveTexture(GL_TEXTURE0);
    //textureId 绑定到GL_TEXTURE0纹理单元上
    glBindTexture(GL_TEXTURE_2D, textureId);
    //第二个参数传的是纹理单元
    glUniform1i(textureIndex, 0);

    glvao->BindVAO();
    glDrawArrays(GL_POINTS, 0, defaultVertexCount);
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PaintShader::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    genPaintTexture(width, height);
    surfaceWidth = width;
    surfaceHeight = height;
}

void PaintShader::genPaintTexture(int drawWidth, int drawHeight) {
    //生成纹理
    glGenTextures(1, &paintTextureId);
    glBindTexture(GL_TEXTURE_2D, paintTextureId);

    //生成FBO
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawWidth, drawHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //使用纹理作为附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, paintTextureId, 0);

    //解绑兄弟
    glBindTexture(GL_TEXTURE_2D, 0);
}