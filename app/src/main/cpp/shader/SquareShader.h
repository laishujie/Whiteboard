//
// Created by Lai on 2021/1/29.
//

#ifndef WHITEBOARD_SQUARESHADER_H
#define WHITEBOARD_SQUARESHADER_H


#include "ShaderBase.h"

class SquareShader : public ShaderBase {
public:
    void Init();

    void draw();

    void OnSurfaceChanged(int width, int height);

    void onDestroy();

    void OnSurfaceChanged(int x, int y, int width, int height);


    float R = 1.0f;
    float G = 1.0f;
};


#endif //WHITEBOARD_SQUARESHADER_H
