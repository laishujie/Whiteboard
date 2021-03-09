//
// Created by Lai on 2021/2/4.
//

#include "TextureImageDemo.h"


void TextureImageDemo::Init() {

    char vShaderStr[] =
            "#version 300 es                          \n"
            "layout(location = 0) in vec4 vPosition;  \n"
            "out vec3 outPos;\n"
            "layout(location = 1) in vec3 uvPos;\n"
            "uniform mat4 uMatrix;\n"
            "out vec3 outUvPos;\n"
            "void main()                              \n"
            "{                                        \n"
            "   gl_Position = uMatrix * vPosition;              \n"
            "   outUvPos = uvPos;              \n"
            "   outPos = vec3(vPosition.x,vPosition.y,vPosition.z);              \n"
            // "gl_PointSize = 50.0; \n"
            "}                                        \n";

    char fboShaderStr[] =
            "#version 300 es                              \n"
            "precision mediump float;                     \n"
            "out vec4 fragColor;                          \n"
            "uniform sampler2D textureMap;\n"
            "in vec3 outPos;\n"
            "in vec3 outUvPos;\n"
            "void main()                                  \n"
            "{                                            \n"
            "vec2 uv = vec2(outUvPos.x,outUvPos.y); \n"
            "vec4 textureMap = texture(textureMap,uv); \n"
            // "   fragColor = vec4 ( outPos, 1.0 ); \n"
            "   fragColor = textureMap;\n"
            "}                                            \n";

    glProgram = new GLProgram(vShaderStr, fboShaderStr);
    glvao = new GLVAO();

    GLfloat rectangleVertices[] = {
            -1.0f, 1.0f, 0.0f,// 左上角
            1.0f, 1.0f, 0.0f,//右上角
            1.0f, -1.0f, 0.0f,//右下角
            -1.0f, -1.0f, 0.0f};//左下角

    //y纹理颠倒
    GLfloat uv[] = {
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            0.0f, 1.0, 0, 0
    };


    unsigned int index[] = {0, 1, 2, 2, 3, 0};


    glvao->AddVertex3D(rectangleVertices, 4, 0);
    glvao->AddVertex3D(uv, 4, 1);
    glvao->setIndex(index, 6);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //LOGCATE("===== textureId %d", textureId);
    //像素偏移1
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 imaData);
    glBindTexture(GL_TEXTURE_2D, 0);


    GLfloat lineVertices[] = {
            -1.0f, 0.0f, 0.0f,
            1.0, -0, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,

            0.0f,0.0f,-1.0f,
            0.0f,0.0f,1.0f
    };

    glLineWidth(10.0f);

    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
    glBindVertexArray(lineVAO);
    //绑定对象 GL_ARRAY_BUFFER：可以用来保存glVertexAttribPointer()设置的顶点数组数据
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    //为当前VBO塞点数据 GL_STATIC_DRAW 表示数据不会或几乎不会改变。
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void TextureImageDemo::draw() {
    //glViewport(0,0,surfaceWidth,surfaceHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //LOGE("11111", "ResultShader::draw %d", resultTextureId);
    glProgram->useProgram();

    GLint textureIndex = glGetUniformLocation(glProgram->program, "textureMap");
    //激活纹理单元编号和纹理单元对应
    glActiveTexture(GL_TEXTURE0);
    //textureId 绑定到GL_TEXTURE0纹理单元上
    glBindTexture(GL_TEXTURE_2D, textureId);
    //第二个参数传的是纹理单元
    glUniform1i(textureIndex, 0);
    GLint transformLoc = glGetUniformLocation(glProgram->program, "uMatrix");


    //glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(0.5f, 0.6f, 0.0f));
    // myMatrix = glm::scale(myMatrix,glm::vec3(0.1f, 0.2f, 0.0f));
    //LOGE("11111","glm::mat4(1.0f) %s",glm::to_string(mMatrix).c_str());
    //mMatrix = glm::translate(mMatrix, glm::vec3(0.5f, 0.6f, 0.0f));
    // mMatrix = glm::scale(mMatrix,glm::vec3(0.1f, 0.2f, 0.0f));

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(mInitMatrix));

    glvao->BindVAO();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 6);
}

void TextureImageDemo::OnSurfaceChanged(int width, int height) {
    glProgram->OnSurfaceChanged(0, 0, width, height);
    surfaceWidth = width;
    surfaceHeight = height;
}

void TextureImageDemo::change(float x, float y, float z) {
    /*float aspect_ratio = (float)surfaceHeight / surfaceWidth;
    glm::mat4 transform;
    transform = glm::rotate(transform, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(1.0f, aspect_ratio, 1.0f));
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -aspect_ratio,aspect_ratio, 0.1f, 0.0f)* transform;*/

    //glm::mat4 Projection = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 4.0f, 100.0f);
    //glm::mat4 Projection = glm::perspective(45.0f,ratio, 0.1f,100.f);
    // View matrix
    /*glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    float radiansX = static_cast<float>(MATH_PI / 180.0f * x);
    float radiansY = static_cast<float>(MATH_PI / 180.0f * y);
    float radiansZ = static_cast<float>(MATH_PI / 180.0f * z);
*/
    /*  // Model matrix
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
      Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
      Model = glm::rotate(Model, radiansZ, glm::vec3(0.0f, 0.0f, 1.0f));
      //Model = glm::scale(Model, glm::vec3(0.5f, 0.5f, 1.0f));
       Model = glm::scale(Model, glm::vec3(m_ScaleX, m_ScaleY, 1.0f));
       Model = glm::rotate(Model, radiansX, glm::vec3(1.0f, 0.0f, 0.0f));
       Model = glm::rotate(Model, radiansY, glm::vec3(0.0f, 1.0f, 0.0f));
       Model = glm::translate(Model, glm::vec3(transX, transY, 0.0f));*/
    //mInitMatrix = Projection;// * View * Model;


    /*float aspect_ratio = (float)surfaceHeight / surfaceWidth;
    glm::mat4 transform;
    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(1.0f, aspect_ratio, 1.0f));
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -aspect_ratio,aspect_ratio, 0.1f, 0.0f)* transform;*/

    float aspect_ratio = (float)surfaceHeight / surfaceWidth;
    /*glm::mat4 projection;
    projection = glm::perspective(45.0f, (GLfloat) surfaceWidth / (GLfloat) surfaceHeight, 0.1f,
                                  100.0f);*/
   // lm::mat4 Projection = glm::frustum(-aspect_ratio, aspect_ratio, -1.0f, 1.0f, 1.0f, 100.0f);
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f,1.0f, 1.0f, 100.0f);

    glm::mat4 View = glm::lookAt(
            glm::vec3(x, y, z), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));


    mInitMatrix = Projection * View * Model;


    draw();
}


void TextureImageDemo::onDestroy() {

}
