#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>
#include <stdbool.h>

int init_image();
uint8_t *loadImage(char *path, size_t *width, size_t *height, int *nrChannels, bool flipImage);

#endif