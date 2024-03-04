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