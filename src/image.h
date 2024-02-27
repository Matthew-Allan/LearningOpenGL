#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define IM_NONE 1
#define IM_HORIZONTAL 1
#define IM_VERTICAL 2

typedef struct
{
    size_t w;
    size_t h;
    int c;
    uint8_t *data;
} Image;


int init_image();
Image *loadImage(char *path, bool flip);
void freeImage(Image *image);

#endif