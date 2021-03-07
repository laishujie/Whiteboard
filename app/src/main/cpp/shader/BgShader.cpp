//
// Created by Lai on 2020/11/7.
//

#include "BgShader.h"
#include <GLES3/gl3.h>
//#include <logUtil.h>
#include <GLUtil.h>
#include <logUtil.h>


void BgShader::draw() {
    if (m_ProgramObj == 0)
        return;
    //使用
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glUseProgram(m_ProgramObj);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BgShader::testDraw1() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(m_ProgramObj);
    glViewport(0, 0, 200, 200);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BgShader::testDraw2() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(100, 300, 200, 200);
    glUseProgram(m_ProgramObj);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void BgShader::genBgTexture(int drawWidth, int drawHeight) {
    //生成纹理
    glGenTextures(1, &bgTextureId);
    glBindTexture(GL_TEXTURE_2D, bgTextureId);

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
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bgTextureId, 0);
    //解绑兄弟
    glBindTexture(GL_TEXTURE_2D, 0);






    //LOGE("11111","glGenTextures %d ",bgTextureId)
}

void BgShader::glClearBg() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

BgShader::~BgShader() {
    if (frameBuffer != 0)
        glDeleteFramebuffers(1, &frameBuffer);
    if (bgTextureId != 0)
        glDeleteTextures(1, &bgTextureId);
    frameBuffer = 0;
    bgTextureId = 0;

    glUseProgram(GL_NONE);

}

void BgShader::Init() {
    if (m_ProgramObj != 0) return;

    if (m_ProgramObj != 0)
        return;
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "gl_PointSize = 50.0; \n"
            "}                                        \n";

    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );  \n"
            "}                                            \n";

    m_ProgramObj = GLUtil::CreateProgram(vShaderStr, fShaderStr, m_VertexShader, m_FragmentShader);

    //LOGE("11111","bgdemo m_ProgramObj %d",m_ProgramObj);
}


void BgShader::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    genBgTexture(width, height);
}


