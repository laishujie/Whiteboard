//
// Created by Lai on 2021/1/8.
//

#include <logUtil.h>
#include "ResultShader.h"

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
            // "   gl_Position = uMatrix * vPosition;              \n"
            "   gl_Position = vPosition;              \n"
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
    LOGE("11111", "ResultShader glvao ");
}

void ResultShader::OnSurfaceChanged(int width, int height) {
    glProgram->OnSurfaceChanged(0, 0, width, height);
    surfaceWidth = width;
    surfaceHeight = height;
}

void ResultShader::mergeTexture(GLuint frameBuffer, GLuint textureId) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    draw(textureId, true);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ResultShader::draw(GLuint resultTextureId, bool isBlend) {
    if (resultTextureId == 0) return;
    if (!isBlend){
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_BLEND);
    }
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
    //GLint transformLoc = glGetUniformLocation(glProgram->program, "uMatrix");
    //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(textureLayer.getMatrix()));
    glvao->BindVAO();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void ResultShader::draw() {}

void ResultShader::onDestroy() {


}
