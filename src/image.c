#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "image.h"
#include <glad/glad.h>

#define SUPPORTED_IMAGE_TYPES IMG_INIT_PNG
#define GET_SIZE(width, height, channels) ((width) * (height) * (channels))
#define GET_IMAGE_SIZE(image) ((image)->w * (image)->h * (image)->c)
#define GET_ROW_OFFSET(y, width, channels) ((y) * (width) * (channels))
#define GET_FL_ROW_OFFSET(y, width, height, channels) (((height) - (y)-1) * (width) * (channels))
#define GET_COL_OFFSET(x, channels) ((x) * (channels))
#define GET_OFFSET(x, y, width, channels) (GET_ROW_OFFSET(y, width, channels) + GET_COL_OFFSET(x, channels))

int init_image()
{
    int error = IMG_Init(SUPPORTED_IMAGE_TYPES) - SUPPORTED_IMAGE_TYPES;
    if (error < 0)
        printf("SDL_image could not init. Error: %s\n", IMG_GetError());
    return error;
}

void flipImage(Image *image)
{
    size_t dataSize = GET_IMAGE_SIZE(image);

    uint8_t data[dataSize];

    for (int i = 0; i < image->h; i++)
        memcpy(data + GET_ROW_OFFSET(i, image->w, image->c), image->data + GET_FL_ROW_OFFSET(i, image->w, image->h, image->c), GET_SIZE(image->w, 1, image->c));

    memcpy(image->data, data, dataSize);
}

Image *loadImage(char *path, bool flip)
{
    // Load image at specified path
    SDL_Surface *surface = IMG_Load(path);
    if (surface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    Image *image = (Image *)malloc(sizeof(Image));
    image->w = surface->w;
    image->h = surface->h;
    image->c = surface->format->BytesPerPixel;
    image->next = NULL;

    size_t dataSize = GET_IMAGE_SIZE(image);

    image->data = (uint8_t *)malloc(dataSize);

    memcpy(image->data, surface->pixels, dataSize);

    SDL_FreeSurface(surface);

    if (flip)
        flipImage(image);

    return image;
}

Image *freeImage(Image *image)
{
    Image *next = image->next;
    free(image->data);
    free(image);
    return next;
}

Image *freeImages(Image *images, ssize_t count)
{
    Image *image = images;
    for(; count != 0 && image != NULL; count -= count > 0)
        image = freeImage(image);
    return image;
}