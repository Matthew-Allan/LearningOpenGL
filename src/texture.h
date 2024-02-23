#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdlib.h>

GLuint createTexture(size_t width, size_t height, uint8_t *data);

#endif