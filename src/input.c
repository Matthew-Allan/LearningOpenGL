#include "input.h"
#include "string.h"

int checkKey(SDL_KeyCode *keys, int length,  SDL_KeyCode code)
{
    for(int i = 0; i < length; i++)
        if(code == keys[i])
            return 1;
    return 0;
}

Axis *freeAxis(Axis *axis)
{
    Axis *next = axis->next;
    free(axis);
    return next;
}

Axis *freeAxes(Axis *axes, size_t count)
{
    Axis *axis = axes;
    for(; count != 0 && axis != NULL; count -= count > 0)
        axis = freeAxis(axis);
    return axis;
}

Axis *createAxis(char *name, int positiveCount, SDL_Scancode positive[5], int negativeCount, SDL_Scancode negative[5])
{
    Axis *axis = (Axis *)malloc(sizeof(Axis));
    axis->name[0] = '\0';
    strncat(axis->name, name, 63);
    axis->positiveCount = positiveCount;
    axis->negativeCount = negativeCount;
    memcpy(axis->positive, positive, sizeof(SDL_Scancode) * 5);
    memcpy(axis->negative, negative, sizeof(SDL_Scancode) * 5);
    axis->next = NULL;
    return axis;
}

int addAxis(Input *input, Axis *axis)
{
    for(Axis *existingAxis = input->axes; existingAxis != NULL; existingAxis = existingAxis->next)
        if(!strcmp(axis->name, existingAxis->name))
            return 1;
    axis->next = input->axes;
    input->axes = axis;
    return 0;
}

Axis *getAxis(Input *input, char *name)
{
    for(Axis *axis = input->axes; axis != NULL; axis = axis->next)
        if(!strcmp(axis->name, name))
            return axis;
    return NULL;
}

int getAxisValue(Input *input, char *name)
{
    int value = 0;
    Axis *axis = getAxis(input, name);
    for(int i = 0; i < axis->positiveCount; i++)
        value += input->keyboardState[axis->positive[i]];
    for(int i = 0; i < axis->negativeCount; i++)
        value -= input->keyboardState[axis->negative[i]];
    return (value == 0) ? 0 : value / abs(value);
}

vec2 get2DAxisValue(Input *input, char *nameX, char *nameY)
{
    return vec2(getAxisValue(input, nameX), getAxisValue(input, nameY));
}
vec3 get3DAxisValue(Input *input, char *nameX, char *nameY, char *nameZ)
{
    return vec3(getAxisValue(input, nameX), getAxisValue(input, nameY), getAxisValue(input, nameZ));
}

Input *setUpInput()
{
    Input *input = (Input *)malloc(sizeof(input));
    input->keyboardState = SDL_GetKeyboardState(NULL);
    input->axes = NULL;
    input->mouseXDelta = 0;
    input->mouseYDelta = 0;
    input->scrollDelta = 0;
    return input;
}

void freeInput(Input *input)
{
    freeAxes(input->axes, -1);
    free(input);
}