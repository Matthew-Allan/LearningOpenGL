#ifndef CAMERA_H
#define CAMERA_H

#include "transforms.h"

typedef struct
{
    vec3 pos;
    vec3 target;
    vec3 dir;
} Camera;

#endif