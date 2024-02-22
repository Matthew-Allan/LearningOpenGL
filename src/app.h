#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
    bool running;
    SDL_Window *window;
    char *path;
} App;

App *setUpApp(SDL_Window *window);
int closeApp(App *app, int code);
char *readResource(char *relativePath, App *app);

#endif