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
    vec3 dir;
    vec3 up;
    vec3 worldUp;
    float aspect;
    PROJECTION projectionType;
    mat4 view;
    mat4 projection;
} Camera;

Camera *createCamera(vec3 pos, vec3 dir, float aspect, PROJECTION projectionType);
void setAspect(Camera *camera, float aspect);
void setCamPos(Camera *camera, vec3 pos);
void setCamTarget(Camera *camera, vec3 target);
void setCamDir(Camera *camera, vec3 dir);
void setCamFOV(Camera *camera, float fov);

#endif