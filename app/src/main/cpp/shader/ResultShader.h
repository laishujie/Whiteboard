//
// Created by Lai on 2021/1/8.
//

#ifndef OPENGLDEMO_RESULTSHADER_H
#define OPENGLDEMO_RESULTSHADER_H


#include "ShaderBase.h"

class ResultShader : public ShaderBase {
public:
    void Init();

    void OnSurfaceChanged(int width, int height);

    void draw();

    void onDestroy();


    void draw(GLuint resultTextureId, bool isBlend);

    void mergeTexture(GLuint frameBuffer, GLuint textureId);
};


#endif //OPENGLDEMO_RESULTSHADER_H
