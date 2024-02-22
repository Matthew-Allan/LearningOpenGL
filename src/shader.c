#include <stdio.h>
#include <string.h>

#include "shader.h"

GLuint loadShader(char *relativePath, App *app, int type)
{
    printf("Loading/Compiling %s\n", relativePath);

    char *source;
    if ((source = readResource(relativePath, app)) == NULL)
    {
        printf("Failed to load shader file. (\"%s\")\n", relativePath);
        return 0;
    }

    printf("%s\n", source);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char **)&source, NULL);
    glCompileShader(shader);
    free(source);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed (\"%s\"): Error %s\n", relativePath, infoLog);
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
