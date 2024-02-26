#include "texture.h"

GLuint createTexture(size_t width, size_t height, int colourMode, int filterMode, int edge, uint8_t *data)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edge);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

    glTexImage2D(GL_TEXTURE_2D, 0, colourMode, width, height, 0, colourMode, GL_UNSIGNED_BYTE, data);

    return texture;
}