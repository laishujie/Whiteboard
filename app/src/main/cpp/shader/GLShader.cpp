//
// Created by Lai on 2020/12/3.
//

#include <malloc.h>
#include <logUtil.h>
#include "CommGL.hpp"

GLShader::GLShader(char *shaderStr, GLShaderType type) {
    if (type == GLShaderType::SHADER_VERTEX) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    else if(type == GLShaderType::SHADER_FRAGMENT){
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }

    glShaderSource(shader, 1, &shaderStr, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    GLint logLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        char* log = (char*)malloc(logLen);
        glGetShaderInfoLog(shader, logLen, NULL, log);

        printf("Shader Log:\n");
        printf("%s\n", log);
        LOGCATE("Shader Log:\n");
        LOGCATE("%s",log);
        free(log);
    }
}

GLShader::~GLShader()
{
    if (shader != 0) {
        glDeleteShader(shader);
        shader = 0;
    }
}