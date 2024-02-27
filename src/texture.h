#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stdlib.h>
#include "image.h"

GLuint createTexture(Image *image, int colourMode, int filterMode, int edge);

#endif