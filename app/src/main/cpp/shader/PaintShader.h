//
// Created by Lai on 2021/1/8.
//

#ifndef OPENGLDEMO_PAINTSHADER_H
#define OPENGLDEMO_PAINTSHADER_H


#include <ImageInfo.h>
#include "ShaderBase.h"

class PaintShader : public ShaderBase {
public:
    ~PaintShader();
    void draw();
    void OnSurfaceChanged(int width, int height);
    void Init();
    void onDestroy();
    void glClearPaint();


    void glUpdatePoints(float *points, int vertexCount);
    void glSetBrush(ImageInfo *brushImageInfo);


    GLuint textureId=0;
    GLuint paintTextureId;
    GLuint frameBuffer;
    GLuint paintFbo;
    int defaultVertexCount = 0;
    ImageInfo *brushImageInfo = nullptr;

    void genPaintTexture(int drawWidth, int drawHeight);
};


#endif //OPENGLDEMO_PAINTSHADER_H
