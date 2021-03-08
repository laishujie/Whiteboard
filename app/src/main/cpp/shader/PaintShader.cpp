//
// Created by Lai on 2021/1/8.
//

#include <logUtil.h>
#include "PaintShader.h"


void PaintShader::Init() {
    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "uniform float brushSize;"
            "float timeStamp;\n"
            "out mat2  rotation_matrix;\n"
            "float random(float val)\n"
            "{\n"
            "return fract(sin(val * 12.9898) * 43756.5453123 );\n"
            "}"
            "void main()                              \n"
            "{                                        \n"
            "timeStamp = random((vPosition.x+vPosition.y)) * 100.0;\n"
            "float sin_theta = sin(timeStamp);\n"
            "    float cos_theta = cos(timeStamp);\n"
            "    rotation_matrix =  mat2(cos_theta, sin_theta,\n"
            "                           -sin_theta, cos_theta);\n"
            "gl_Position =  vPosition;              \n"
            "gl_PointSize = brushSize; \n"
            "}                                        \n";


    char fShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "in mat2 rotation_matrix;\n"
            "uniform mat4 uMatrix;\n"
            "uniform sampler2D textureMap;\n"
            "uniform float isSupportRotate;\n"
            "uniform vec4 outColor;\n"
            "float outColorTransparent;"
            "float aTransparent;"
            "void main()                                  \n"
            "{                                            \n"
            "vec4 mask = vec4(0.);"
            "if(isSupportRotate==0.0){"
            "mask = texture(textureMap, vec2(gl_PointCoord.x,gl_PointCoord.y));"
            "}else{"
            "vec2 pt = gl_PointCoord - vec2(0.5);\n"
            "mask = texture(textureMap, rotation_matrix * pt + vec2(0.5));"
            "}"
            "outColorTransparent = outColor.a;"
            "vec3 aTransparentColor=vec3(0.);\n"
            "  if(mask.a<1.0){\n"
            "      aTransparent = mask.a * outColorTransparent;\n"
            "      aTransparentColor = mask.rgb;\n"
            "fragColor = aTransparent *(vec4(1.0) - ((vec4(1.0)-outColor))*(vec4(1.0)-vec4(aTransparentColor,1.0)));\n"
            "}"
            //"else{fragColor = mask;}"
            "else{fragColor = outColorTransparent * (vec4(1.0) - ((vec4(1.0)-vec4(outColor.rgb,1.0)))*(vec4(1.0)-mask));}\n"
            //"else{discard;}\n"

            //discard;
            //fragColor = (vec4(1.0) - ((vec4(1.0)-outColor))*(vec4(1.0)-mask));
            //"vec4 outColor = vec4(0.0, 1.0, 0.0, 0.0);"
            // "   fragColor = 1.0 * vec4(1.0, 0.0, 0.0, 1.0) * mask;\n"
            //blendColor * 1.0 + baseColor * (1.0 - blendColor.a);
            //"fragColor = (vec4(1.0) - ((vec4(1.0)-outColor))*(vec4(1.0)-mask));\n"
            //"fragColor = mask;\n"
            "} \n";

    glProgram = new GLProgram(vShaderStr, fShaderStr);
    glvao = new GLVAO();


    if (brushImageInfo != nullptr && brushImageInfo->imageInfo != nullptr) {
        glGenTextures(1, &brushTextureId);
        glBindTexture(GL_TEXTURE_2D, brushTextureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //像素偏移1
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, brushImageInfo->imageInfo->width,
                     brushImageInfo->imageInfo->height, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     brushImageInfo->imageInfo->pixels);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    //LOGE("11111", "PaintShader glProgram %d", glProgram->program)
}

void PaintShader::setPaintColor(float A, float R, float G, float B) {
    if (brushImageInfo == nullptr) return;
    brushImageInfo->R = R;
    brushImageInfo->B = B;
    brushImageInfo->G = G;
    brushImageInfo->A = A;
}

void PaintShader::glSetBrush(ImageInfo *imageInfo, float brushWidth, bool isTextureRotate,
                             BrushInfo::OutType outType) {
    if (imageInfo != nullptr) {
        if (brushImageInfo->imageInfo != nullptr) {
            delete brushImageInfo->imageInfo;
            brushImageInfo->imageInfo = nullptr;
        }

        if (imageInfo->pixels != nullptr) {
            if (brushTextureId == 0) {
                glGenTextures(1, &brushTextureId);
                glBindTexture(GL_TEXTURE_2D, brushTextureId);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //像素偏移1
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageInfo->width, imageInfo->height, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE,
                             imageInfo->pixels);
            } else {
                glBindTexture(GL_TEXTURE_2D, brushTextureId);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imageInfo->width, imageInfo->height,
                                GL_RGBA, GL_UNSIGNED_BYTE, imageInfo->pixels);
            }

            glBindTexture(GL_TEXTURE_2D, 0);
            //记录
            brushImageInfo->imageInfo = imageInfo;
        }
    }

    if (brushWidth > 0) {
        brushImageInfo->brushSize = brushWidth;
    }

    brushImageInfo->outType = outType;
    brushImageInfo->isTextureRotate = isTextureRotate;
}

PaintShader::~PaintShader() {
    onDestroy();
}

void PaintShader::onDestroy() {
    if (brushImageInfo != nullptr) {
        delete brushImageInfo;
        brushImageInfo = nullptr;
    }
    if (brushTextureId > 0) {
        glDeleteTextures(1, &brushTextureId);
        brushTextureId = 0;
    }

    if (frameBuffer != 0) {
        glDeleteFramebuffers(1, &frameBuffer);
        frameBuffer = 0;
    }
    paintVbo = 0;
    //LOGE("11111", "onDestroy PaintShader")
}

void PaintShader::glUpdatePoints(float *points, int vertexCount, float rotate, bool isClear) {
    //LOGE("11111", "vertexCount %d", vertexCount)
    //textureRotate = rotate;
    if (isClear) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if(points!= nullptr){
        glvao->setVertex2D(paintVbo, points, vertexCount, 0);
        defaultVertexCount = vertexCount;

        draw();
    }
}

void PaintShader::glClearPaint() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PaintShader::draw() {
    if (paintVbo == 0) return;

    glEnable(GL_BLEND);

    if (brushImageInfo != nullptr && brushImageInfo->outType == BrushInfo::ERASER)
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
    else
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


    glBlendEquation(GL_FUNC_ADD);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //glViewport(0, 0, surfaceWidth, surfaceHeight);

    glProgram->useProgram();

    if (brushTextureId > 0) {
        GLint textureIndex = glGetUniformLocation(glProgram->program, "textureMap");
        //激活纹理单元编号和纹理单元对应
        glActiveTexture(GL_TEXTURE0);
        //textureId 绑定到GL_TEXTURE0纹理单元上
        glBindTexture(GL_TEXTURE_2D, brushTextureId);
        //第二个参数传的是纹理单元
        glUniform1i(textureIndex, 0);
    }


    GLint brushSizeIndex = glGetUniformLocation(glProgram->program, "brushSize");
    GLint isSupportRotateIndex = glGetUniformLocation(glProgram->program, "isSupportRotate");

    if (brushImageInfo != nullptr) {
        glUniform1f(brushSizeIndex, brushImageInfo->brushSize);

        GLint ourColorIndex = glGetUniformLocation(glProgram->program, "outColor");
        glUniform4f(ourColorIndex, brushImageInfo->R, brushImageInfo->G, brushImageInfo->B,
                    brushImageInfo->A);
        glUniform1f(isSupportRotateIndex, brushImageInfo->isTextureRotate ? 1.0f : 0.0f);
        //glUniform1f(aColorIndex, brushImageInfo->A);
        // glUniform1f(aColorIndex, 0.1f);
        // glUniform4f(ourColorIndex, 0.59607846f, 0.07058824f, 0.07058824f,0.1f);
    } else {
        glUniform1f(brushSizeIndex, 15.0f);
        glUniform1f(isSupportRotateIndex, 0.0f);
    }
    /*glm::mat4 r;
    glm::rotate(r, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    LOGE("11111", "Log:%s", glm::to_string(r).c_str());
    GLint uMatrixIndex = glGetUniformLocation(glProgram->program, "uMatrix");
    glUniformMatrix4fv(uMatrixIndex, 1, GL_FALSE, glm::value_ptr(r));*/

    glvao->BindVAO();
    glDrawArrays(GL_POINTS, 0, defaultVertexCount);

    glDisable(GL_BLEND);
}

void PaintShader::OnSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    genPaintTexture(width, height);
    surfaceWidth = width;
    surfaceHeight = height;
}


void PaintShader::genPaintTexture(int drawWidth, int drawHeight) {
    //生成纹理
    glGenTextures(1, &paintTextureId);
    glBindTexture(GL_TEXTURE_2D, paintTextureId);

    //生成FBO
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, drawWidth, drawHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //使用纹理作为附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, paintTextureId, 0);

    //解绑兄弟
    glBindTexture(GL_TEXTURE_2D, 0);
}