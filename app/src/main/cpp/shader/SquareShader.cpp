//
// Created by Lai on 2021/1/29.
//

#include "SquareShader.h"

void SquareShader::Init() {

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = vPosition;              \n"
            "}                                        \n";


    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "uniform float R;  \n"
            "uniform float G;  \n"
            "void main()                                  \n"
            "{                                            \n"
            "   fragColor = vec4 ( R, G, 0.0, 1.0 );  \n"
            "}                                            \n";

    glProgram = new GLProgram(vShaderStr, fShaderStr);
    glvao = new GLVAO();


    GLfloat rectangleVertices[] = {
            -1.0f, 1.0f, 0.0f,// 左上角
            1.0f, 1.0f, 0.0f,//右上角
            1.0f, -1.0f, 0.0f,//右下角
            -1.0f, -1.0f, 0.0f};//左下角

    //y纹理颠倒
    /*GLfloat uv[] = {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0f, 1.0, 0, 0
    };*/

    unsigned int index[] = {0, 1, 2, 2, 3, 0};
    glvao->AddVertex3D(rectangleVertices, 4, 0);
    glvao->setIndex(index, 6);
}

void SquareShader::draw() {
    glProgram->useProgram();
    glUniform1f(glGetUniformLocation(glProgram->program, "R"), R);
    glUniform1f(glGetUniformLocation(glProgram->program, "G"), G);
    glvao->BindVAO();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SquareShader::onDestroy() {

}

void SquareShader::OnSurfaceChanged(int width, int height) {
    glViewport(0, height / 2, width / 2, width / 2);
}

void SquareShader::OnSurfaceChanged(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}