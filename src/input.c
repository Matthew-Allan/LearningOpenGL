#include "input.h"

int checkKey(SDL_KeyCode *keys, int length,  SDL_KeyCode code)
{
    for(int i = 0; i < length; i++)
        if(code == keys[i])
            return 1;
    return 0;
}

void takeAxisKey(Axis *axis, SDL_KeyCode code)
{
    if (checkKey(axis->negative, axis->negativeCount, code))
    {
        axis->value--;
        return;
    }
    if (checkKey(axis->positive, axis->positiveCount, code))
        axis->value++;
}