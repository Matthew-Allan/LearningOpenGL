#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

#define SUPPORTED_IMAGE_TYPES IMG_INIT_PNG

int init_image()
{
    int error = IMG_Init(SUPPORTED_IMAGE_TYPES) - SUPPORTED_IMAGE_TYPES;
    if (error < 0)
        printf("SDL_image could not init. Error: %s\n", IMG_GetError());
    return error;
}

uint8_t *loadImage(char *path, size_t *width, size_t *height, int *nrChannels)
{
    // Load image at specified path
    SDL_Surface *image = IMG_Load(path);
    if (image == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    *nrChannels = image->format->BytesPerPixel;
    *width = image->w;
    *height = image->h;

    size_t dataSize = *width * *height * *nrChannels;

    uint8_t *data = (uint8_t *)malloc(dataSize);
    if(data == NULL)
    {
        printf("Unable to load image. Not enough space\n");
        SDL_FreeSurface(image);
        return NULL;
    }
    
    memcpy(data, image->pixels, dataSize);

    SDL_FreeSurface(image);

    return data;
}