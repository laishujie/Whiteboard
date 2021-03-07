//
// Created by Lai on 2021/1/8.
//

#include <logUtil.h>
#include "ResultShader.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

void ResultShader::Init() {

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "out vec3 outPos;\n"
            "layout(location = 1) in vec3 uvPos;\n"
            "uniform mat4 uMatrix;\n"
            "out vec3 outUvPos;\n"
            "void main()                              \n"
            "{                                        \n"
            "gl_Position = uMatrix * vPosition;              \n"
            //"   gl_Position = vPosition;              \n"
            "   outUvPos = uvPos;              \n"
            "   outPos = vec3(vPosition.x,vPosition.y,vPosition.z);              \n"
            // "gl_PointSize = 50.0; \n"
            "}                                        \n";

    char fboShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "uniform sampler2D textureMap;\n"
            "in vec3 outPos;\n"
            "in vec3 outUvPos;\n"
            "void main()                                  \n"
            "{                                            \n"
            "vec2 uv = vec2(outUvPos.x,1.0-outUvPos.y); \n"
            "vec4 textureMap = texture(textureMap,uv); \n"
            // "   fragColor = vec4 ( outPos, 1.0 ); \n"
            "   fragColor = textureMap;\n"
            "}                                            \n";

    glProgram = new GLProgram(vShaderStr, fboShaderStr);
    glvao = new GLVAO();

    GLfloat rectangleVertices[] = {
            -1.0f, 1.0f, 0.0f,// 左上角
            1.0f, 1.0f, 0.0f,//右上角
            1.0f, -1.0f, 0.0f,//右下角
            -1.0f, -1.0f, 0.0f};//左下角

    //y纹理颠倒
    GLfloat uv[] = {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0f, 1.0, 0, 0
    };

    unsigned int index[] = {0, 1, 2, 2, 3, 0};
    glvao->AddVertex3D(rectangleVertices, 4, 0);
    glvao->AddVertex3D(uv, 4, 1);
    glvao->setIndex(index, 6);
}

void ResultShader::OnSurfaceChanged(int width, int height) {
    glProgram->OnSurfaceChanged(0, 0, width, height);
    surfaceWidth = width;
    surfaceHeight = height;
    //genResultTexture(surfaceWidth, surfaceHeight);
/*
    float left = -1.0f;
    float right = 1.0f;
    float bottom = -1.0f;
    float top = 1.0f;
    float aspectTexture = 16 / (float) 9;
    float aspectPlane = surfaceWidth / (float) surfaceHeight;
    LOGCATE("width %d height %d aspectTexture %f aspectPlane %f",
            surfaceWidth, surfaceHeight, aspectTexture, aspectPlane);

//width 1080 height 1680 aspectTexture 0.642857 aspectPlane 1.000000
//1080
//fitCenter
    //1. 纹理比例 > 投影平面比例 固定宽度修改高度
    if (aspectTexture > aspectPlane) {
        top = 1 / aspectPlane * aspectTexture;
        bottom = -top;
    } else {
        left = -aspectPlane / aspectTexture;
        right = -left;
    }
    LOGCATE("left %f right %f top %f bottom %f",
            left, right, top, bottom);
    //通过正交矩阵裁剪显示范围
    matrix = glm::ortho(left, right, bottom, top);*/
}


void ResultShader::mergeTextureDisplay(GLuint &bgTextureId, GLuint &paintTextureId) {
    //开启混合重叠
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    //切换结果fbo
    /* glBindFramebuffer(GL_FRAMEBUFFER, mResultFrameBuffer);
     glClearColor(0, 0, 0, 0);
     glClear(GL_COLOR_BUFFER_BIT);*/

 /*
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);*/

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //使用背景绘制结果集
    //draw(bgTextureId, mInitMatrix);
    //绘制画笔
    draw(paintTextureId, mInitMatrix);

    glDisable(GL_BLEND);

    //切换到屏幕缓冲区清除数据
    /* glBindFramebuffer(GL_FRAMEBUFFER, 0);
     glClearColor(0, 0, 0, 0);
     glClear(GL_COLOR_BUFFER_BIT);
     glDisable(GL_BLEND);

     //最终在输出背景结果合并纹理
     draw(mResultTextureId, mMatrix);*/
}

ResultShader::~ResultShader() {
    onDestroy();
}

void ResultShader::draw(GLuint resultTextureId, glm::mat4 matrix) {
    if (resultTextureId == 0) return;
    //glViewport(0,0,surfaceWidth,surfaceHeight);

    //LOGE("11111", "ResultShader::draw %d", resultTextureId);
    glProgram->useProgram();

    GLint textureIndex = glGetUniformLocation(glProgram->program, "textureMap");
    //激活纹理单元编号和纹理单元对应
    glActiveTexture(GL_TEXTURE0);
    //textureId 绑定到GL_TEXTURE0纹理单元上
    glBindTexture(GL_TEXTURE_2D, resultTextureId);
    //第二个参数传的是纹理单元
    glUniform1i(textureIndex, 0);
    GLint transformLoc = glGetUniformLocation(glProgram->program, "uMatrix");


    //glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.6f, 0.0f));
    // myMatrix = glm::scale(myMatrix,glm::vec3(0.1f, 0.2f, 0.0f));
    //mMatrix = glm::mat4(1.0f);
    //LOGE("11111","glm::mat4(1.0f) %s",glm::to_string(mMatrix).c_str());
    //mMatrix = glm::translate(mMatrix, glm::vec3(0.5f, 0.6f, 0.0f));
    // mMatrix = glm::scale(mMatrix,glm::vec3(0.1f, 0.2f, 0.0f));
    //mMatrix  = glm::rotate(mMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    //mMatrix = glm::translate(mMatrix, glm::vec3(1.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

    // LOGE("11111", "Log:%s", glm::to_string(mInitMatrix).c_str());

    glvao->BindVAO();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ResultShader::draw() {

}

void ResultShader::onDestroy() {
    if (mResultTextureId > 0) {
        glDeleteTextures(1, &mResultTextureId);
        mResultTextureId = 0;
    }

    if (mResultFrameBuffer != 0) {
        glDeleteFramebuffers(1, &mResultFrameBuffer);
        mResultFrameBuffer = 0;
    }
}

void ResultShader::save(const char *savePath) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    int width = surfaceWidth;
    int height = surfaceHeight;

    unsigned char *buffer = new unsigned char[surfaceHeight * surfaceWidth * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    stbi_flip_vertically_on_write(true);

    if (!stbi_write_png(savePath, width, height, 4, buffer, 0)) {
        LOGE("11111", "ERROR: could not write image");
    }
    delete buffer;
}


void ResultShader::genResultTexture(int drawWidth, int drawHeight) {
    glGenTextures(1, &mResultTextureId);
    glBindTexture(GL_TEXTURE_2D, mResultTextureId);

    //生成FBO
    glGenFramebuffers(1, &mResultFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mResultFrameBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawWidth, drawHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //使用纹理作为附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mResultTextureId,
                           0);

    //解绑兄弟
    glBindTexture(GL_TEXTURE_2D, 0);
}


void ResultShader::setMMatrix(float *f) {
    ResultShader::mMatrix = glm::make_mat4(f);
}

void ResultShader::setTransformation(float r, float restR, float dx, float dy, float sc) {
    float aspect_ratio = (float) surfaceHeight / surfaceWidth;

    if (restR != 0) {
        glm::mat4 transform;
        transform = glm::rotate(mMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMatrix = transform;
        return;
    }

    glm::mat4 transform;
    transform = glm::rotate(transform, glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(1.0f, aspect_ratio, 1.0f));
    // Projection matrix
    glm::mat4 Projection =
            glm::ortho(-1.0f, 1.0f, -aspect_ratio, aspect_ratio, 0.1f, 0.0f) * transform;

    // M' = T(dx, dy) * M.
    glm::mat4 tex = ResultShader::mMatrix * glm::translate(glm::vec3(dx, dy, 0.0f));

    //M' = S(sx, sy, px, py) * M
    glm::mat4 scx2 = tex * glm::scale(glm::vec3(sc, sc, 1.0f)) * Projection;

    ResultShader::mMatrix = scx2;

    //测试

    /*if (dx != 0.0f) {
        ResultShader::mMatrix = glm::translate(ResultShader::mMatrix, glm::vec3(dx, dy, 0.0f));
    } else if (sc != 0.0f) {
        ResultShader::mMatrix = glm::scale(ResultShader::mMatrix, glm::vec3(sc, sc, 1.0f));
    }else{
        float aspect_ratio = (float) surfaceHeight / surfaceWidth;
        glm::mat4 transform;
        transform = glm::rotate(ResultShader::mMatrix, glm::radians(r), glm::vec3(0.0f, 0.0f, 1.0f));

        transform = glm::scale(transform, glm::vec3(1.0f, aspect_ratio, 1.0f));
        // Projection matrix
        glm::mat4 Projection =
                glm::ortho(-1.0f, 1.0f, -aspect_ratio, aspect_ratio, 0.1f, 0.0f) * transform;

        ResultShader::mMatrix = Projection;
    }*/
}
