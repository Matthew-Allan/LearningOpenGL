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
    int w;
    int h;
    char *path;
    Image *images;
} App;

App *setUpApp(SDL_Window *window);
void closeAppWindow(App *app);
int closeApp(App *app, int code);
void updateDimentions(App *app);
void *readResource(char *relativePath, App *app);
Image *readImageRsrc(char *relativePath, App *app, bool flipped);

#endif