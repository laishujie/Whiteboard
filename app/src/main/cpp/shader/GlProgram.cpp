//
// Created by Lai on 2020/12/3.
//

#include "CommGL.hpp"
#include <GLES3/gl3.h>
#include <malloc.h>
#include <logUtil.h>

GLProgram::GLProgram(char *vertexShaderStr, char *fragmentShaderSrc) {
    program = glCreateProgram();

    GLShader vertexShader(vertexShaderStr, GLShaderType::SHADER_VERTEX);
    GLShader fragmentShader(fragmentShaderSrc, GLShaderType::SHADER_FRAGMENT);

    glAttachShader(program, vertexShader.shader);
    glAttachShader(program, fragmentShader.shader);

    glLinkProgram(program);

    GLint logLen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        char *log = (char *) malloc(logLen);
        glGetProgramInfoLog(program, logLen, nullptr, log);

        printf("Program Log:\n");
        printf("%s\n\n", log);
        LOGCATE("Program Log:\n");
        LOGCATE("%s",log);
        free(log);
    }
}

GLProgram::~GLProgram() {
    if (program != 0) {
        glDeleteProgram(program);
        program = 0;
    }
}

int GLProgram::useProgram() {
    glUseProgram(program);
    return 0;
}