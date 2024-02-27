#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "image.h"

typedef struct
{
    bool running;
    SDL_Window *window;
    char *path;
} App;

App *setUpApp(SDL_Window *window);
int closeApp(App *app, int code);
void *readResource(char *relativePath, App *app);
Image *readImageRsrc(char *relativePath, App *app, bool flipped);

#endif