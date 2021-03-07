//
// Created by Lai on 2021/1/8.
//

#ifndef OPENGLDEMO_RESULTSHADER_H
#define OPENGLDEMO_RESULTSHADER_H


#include "ShaderBase.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.inl>
#include <ext.hpp>

class ResultShader : public ShaderBase {
public:
    ResultShader() : mMatrix(1.0f), mInitMatrix(1.0f), mResultTextureId(0), mResultFrameBuffer(0) {}

    ~ResultShader();

    void setMMatrix(float *values);

    void setTransformation(float r, float restR, float dx, float dy, float sc);

    void Init();

    void OnSurfaceChanged(int width, int height);

    void draw();

    void onDestroy();

    void draw(GLuint resultTextureId, glm::mat4 matrix);

    void mergeTextureDisplay(GLuint &bgTextureId, GLuint &paintTextureId);


    void genResultTexture(int drawWidth, int drawHeight);

    void save(const char* savePath);

    glm::mat4 mMatrix;
    glm::mat4 mInitMatrix;
    GLuint mResultTextureId;
    GLuint  mResultFrameBuffer;
};


#endif //OPENGLDEMO_RESULTSHADER_H
