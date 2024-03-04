#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct Axis
{
    char name[64];
    int positiveCount;
    SDL_KeyCode positive[5];
    int negativeCount;
    SDL_KeyCode negative[5];
    int value;
    struct Axis *next;
} Axis;

Axis *freeAxis(Axis *axis);
Axis *freeAxes(Axis *axes, size_t count);
Axis *createAxis(char *name, int positiveCount, SDL_Scancode positive[5], int negativeCount, SDL_Scancode negative[5]);

#endif