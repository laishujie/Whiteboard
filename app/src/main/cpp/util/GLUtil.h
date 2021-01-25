//
// Created by Lai on 2020/11/8.
//

#ifndef OPENGLDEMO_GLUTIL_H
#define OPENGLDEMO_GLUTIL_H
#include <GLES3/gl3.h>

class GLUtil {

public:
    static GLuint CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                                      GLuint &vertexShaderHandle,
                                      GLuint &fragShaderHandle);
    static GLuint LoadShader(GLenum shaderType, const char *pSource);

    static void CheckGLError(const char *pGLOperation);


    static  void  test();
};


#endif //OPENGLDEMO_GLUTIL_H
