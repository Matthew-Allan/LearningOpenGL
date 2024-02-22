#ifndef SHADER_COMP_H
#define SHADER_COMP_H

#include <stdlib.h>
#include <glad/glad.h>

#include "app.h"

GLuint compileShader(char *filepath, int type);
GLuint loadShader(char *relativePath, App *app, int type);
GLuint createProgram(App *app, GLuint shaders[], size_t shaderCount);

#endif