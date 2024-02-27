#include "texture.h"

GLuint createTexture(Image *image, int colourMode, int filterMode, int edge)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    int pixelFormat = (int[]){GL_RED, GL_RG, GL_RGB, GL_RGBA}[image->c - 1];

    glTexImage2D(GL_TEXTURE_2D, 0, colourMode, image->w, image->h, 0, pixelFormat, GL_UNSIGNED_BYTE, image->data);

    return texture;
}