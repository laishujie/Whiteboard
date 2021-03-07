//
// Created by Lai on 2020/12/3.
//

#include "CommGL.hpp"
#include <GLES3/gl3.h>
#include <logUtil.h>

GLuint GLVAO::AddVertex3D(float *data, int vertexCount, GLuint layout) {
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *) 0);

    glEnableVertexAttribArray(layout);

    vboList.push_back(vbo);

    glBindVertexArray(0);
    return vbo;
}

GLuint GLVAO::AddVertex2D(float *data, int vertexCount, GLuint layout) {
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);


    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(layout, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid *) 0);

    glEnableVertexAttribArray(layout);

    vboList.push_back(vbo);

    glBindVertexArray(0);
    return vbo;
}


void GLVAO::updateVertex2D(GLuint updateFbo, float *data, int vertexCount, GLuint layout) {
    glBindBuffer(GL_ARRAY_BUFFER, updateFbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), data, GL_DYNAMIC_DRAW);
   // LOGE("11111", "data %f vertexCount %d updateFbo %d", *data, vertexCount, updateFbo);
}

GLuint GLVAO::setVertex2D(GLuint& fbo, float *data, int vertexCount, GLuint layout) {
    //LOGE("11111","FBO %d",fbo)
    if (fbo == 0) {
        fbo =  AddVertex2D(data, vertexCount, layout);
    } else {
        updateVertex2D(fbo, data, vertexCount, layout);
    }
    return fbo;
}


GLVAO::GLVAO() {
    glGenVertexArrays(1, &vao);
}

//绑定ebo
void GLVAO::setIndex(unsigned int *indexData, int indexCount) {
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData,
                 GL_STATIC_DRAW);
    GLVAO::indexCount = indexCount;
    glBindVertexArray(0);
}

int GLVAO::BindVAO() {
    glBindVertexArray(vao);
    return 0;
}

GLVAO::~GLVAO() {
    for (int i = 0; i < vboList.size(); i++) {
        GLuint vbo = vboList[i];
        glDeleteBuffers(1, &vbo);
    }
    vboList.clear();

    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
}