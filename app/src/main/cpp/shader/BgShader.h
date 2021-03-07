//
// Created by Lai on 2020/11/21.
//

#ifndef OPENGLDEMO_BGDEMO_H
#define OPENGLDEMO_BGDEMO_H

#include <GLES3/gl3.h>


class BgShader {
public:
    BgShader() {
        m_ProgramObj = 0;
        m_VertexShader = 0;
        m_FragmentShader = 0;
    };

    ~BgShader();

    void Init();

    void draw();

    void OnSurfaceChanged(int width, int height);

    void genBgTexture(int drawWidth, int drawHeight);

    void glClearBg();

    void testDraw1();
    void testDraw2();

    GLuint m_ProgramObj;
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    //输出背景纹理
    GLuint bgTextureId = 0;
    //FBO
    GLuint frameBuffer = 0;
};


#endif //OPENGLDEMO_BGDEMO_H
