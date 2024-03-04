#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

typedef struct
{
    int positiveCount;
    SDL_KeyCode positive[5];
    int negativeCount;
    SDL_KeyCode negative[5];
    int value; 
} Axis;

void takeAxisKey(Axis *axis, SDL_KeyCode code);

#endif