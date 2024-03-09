#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "transforms.h"

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

typedef struct
{
    Axis *axes;
    int mouseXDelta;
    int mouseYDelta;
    int scrollDelta;
    const Uint8 *keyboardState;
} Input;

Input *setUpInput();
void freeInput(Input *input);
Axis *freeAxis(Axis *axis);
Axis *freeAxes(Axis *axes, size_t count);
Axis *createAxis(char *name, int positiveCount, SDL_Scancode positive[5], int negativeCount, SDL_Scancode negative[5]);
int addAxis(Input *input, Axis *axis);
int getAxisValue(Input *input, char *name);
vec2 get2DAxisValue(Input *input, char *nameA, char *nameB);
vec3 get3DAxisValue(Input *input, char *nameA, char *nameB, char *nameC);

#endif