#ifndef CAMERA_H
#define CAMERA_H

#include "transforms.h"

typedef enum
{
    PERSPECTIVE,
    ORTHOGRAPHIC
} PROJECTION;

typedef struct
{
    vec3 pos;
    vec3 target;
    vec3 up;
    float aspect;
    PROJECTION projectionType;
    mat4 view;
    mat4 projection;
} Camera;

Camera *createCamera(vec3 pos, vec3 target, float aspect, PROJECTION projectionType);
void setAspect(Camera *camera, float aspect);
void setCamPos(Camera *camera, vec3 pos);
void setCamTarget(Camera *camera, vec3 target);

#endif