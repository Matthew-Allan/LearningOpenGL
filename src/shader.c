#include <stdio.h>
#include <string.h>

#include "shader.h"

GLuint loadShader(char *source, int type)
{
    printf("Loading shader\nSource:\n%s\n", source);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed: Error %s\n", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(GLuint shaders[], size_t shaderCount)
{
    GLuint shaderProgram = glCreateProgram();

    for (int i = 0; i < shaderCount; i++)
        glAttachShader(shaderProgram, shaders[i]);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader linking failed: Error %s\n", infoLog);
        glDeleteProgram(shaderProgram);
        return 0;
    }

    return shaderProgram;
}
