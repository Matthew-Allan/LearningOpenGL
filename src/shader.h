#ifndef SHADER_COMP_H
#define SHADER_COMP_H

#include <stdlib.h>
#include <glad/glad.h>

#include "app.h"

GLuint loadShader(char *filepath, App *app, int type);
GLuint createProgram(GLuint shaders[], size_t shaderCount);

#endif