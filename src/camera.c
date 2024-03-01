#include <stdlib.h>

#include "camera.h"

Camera *createCamera(vec3 pos, vec3 target)
{
    Camera *camera = (Camera *)malloc(sizeof(Camera));
    camera->pos = pos;
    camera->target = target;
    norm3(sub3(), camera->dir);
}