#include <SDL2/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "image.h"

#define SUPPORTED_IMAGE_TYPES IMG_INIT_PNG
#define GET_SIZE(width, height, channels) ((width) * (height) * (channels))
#define GET_ROW_OFFSET(y, width, channels) ((y) * (width) * (channels))
#define GET_FL_ROW_OFFSET(y, width, height, channels) (((height) - (y) - 1) * (width) * (channels))
#define GET_COL_OFFSET(x, channels) ((x) * (channels))
#define GET_OFFSET(x, y, width, channels) (GET_ROW_OFFSET(y, width, channels) + GET_COL_OFFSET(x, channels))

int init_image()
{
    int error = IMG_Init(SUPPORTED_IMAGE_TYPES) - SUPPORTED_IMAGE_TYPES;
    if (error < 0)
        printf("SDL_image could not init. Error: %s\n", IMG_GetError());
    return error;
}

uint8_t *loadImage(char *path, size_t *width, size_t *height, int *nrChannels, bool flipImage)
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
    
    if(!flipImage)
        memcpy(data, image->pixels, dataSize);
    else
        for(int i = 0; i < *height; i++)
            memcpy(data + GET_ROW_OFFSET(i, *width, *nrChannels), image->pixels + GET_FL_ROW_OFFSET(i, *width, *height, *nrChannels), GET_SIZE(*width, 1, *nrChannels));

    SDL_FreeSurface(image);

    return data;
}