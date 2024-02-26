#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdlib.h>

GLuint createTexture(size_t width, size_t height, int colourMode, int filterMode, int edge, uint8_t *data);

#endif