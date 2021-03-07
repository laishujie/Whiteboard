//
// Created by Lai on 2021/1/8.
//

#ifndef OPENGLDEMO_PAINTSHADER_H
#define OPENGLDEMO_PAINTSHADER_H


#include <ImageInfo.h>
#include "ShaderBase.h"

#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.inl>
#include <ext.hpp>

class PaintShader : public ShaderBase {
/*
private:
    float textureRotate= 45.0f;*/

public:
    PaintShader() : brushTextureId(0), paintTextureId(0), paintVbo(0), defaultVertexCount(0),
                    brushImageInfo(new BrushInfo()) {};

    ~PaintShader();

    void draw();

    void OnSurfaceChanged(int width, int height);

    void Init();

    void onDestroy();

    void glClearPaint();

    void glUpdatePoints(float *points, int vertexCount, float textureRotate, bool isClear);

    void glSetBrush(ImageInfo *brushImageInfo,
                    float brushWidth,
                    bool isTextureRotate,
                    BrushInfo::OutType outType
    );

    void setPaintColor(float A,
                       float R,
                       float G,
                       float B);

    GLuint brushTextureId;
    GLuint paintTextureId;
    GLuint frameBuffer;
    GLuint paintVbo;
    int defaultVertexCount;
    BrushInfo *brushImageInfo;



    //glm::mat4 matrix;


    void genPaintTexture(int drawWidth, int drawHeight);
};


#endif //OPENGLDEMO_PAINTSHADER_H
