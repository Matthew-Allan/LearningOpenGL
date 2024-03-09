#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include "image.h"
#include "input.h"

#define ASPECT(app) ((float)((App*)(app))->w / (float)((App*)(app))->h)

typedef struct
{
    bool running;
    SDL_Window *window;
    int w;
    int h;
    char *path;
    Image *images;
    Input *input;
    Uint64 prevTime;
    Uint64 deltaTime;
    Uint64 prevSec;
    int frames;
    int fps;
} App;

App *setUpApp(SDL_Window *window);
void closeAppWindow(App *app);
void closeApp(App *app);
void updateDimentions(App *app);
void *readResource(char *relativePath, App *app);
Image *readImageRsrc(char *relativePath, App *app, bool flipped);
void tickFrame(App *app);

#endif