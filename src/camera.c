#include <stdlib.h>

#include "camera.h"

void setProjection(Camera *camera)
{
    camera->projection = identMat4;
    if (camera->projectionType == PERSPECTIVE)
        perspective(&camera->projection, rad(45), camera->aspect, 0.1f, 100.0f);
    else if (camera->projectionType == ORTHOGRAPHIC)
        orthographic(&camera->projection, -camera->aspect * 6, camera->aspect * 6, -6, 6, 0.1, 100);
}

void updateView(Camera *camera)
{
    lookAt(camera->pos, camera->target, camera->up, &camera->view);
}

Camera *createCamera(vec3 pos, vec3 target, float aspect, PROJECTION projectionType)
{
    Camera *camera = (Camera *)malloc(sizeof(Camera));
    camera->pos = pos;
    camera->target = target;
    camera->up = vec3(0, 1, 0);
    camera->aspect = aspect;
    camera->projectionType = projectionType;

    updateView(camera);

    setProjection(camera);

    return camera;
}

void setCamPos(Camera *camera, vec3 pos)
{
    camera->pos = pos;

    updateView(camera);
}

void setCamTarget(Camera *camera, vec3 target)
{
    camera->target = target;

    updateView(camera);
}

void setCamDir(Camera *camera, vec3 dir)
{
    add3(&dir, &camera->pos, &camera->target, 1);

    updateView(camera);
}

void setAspect(Camera *camera, float aspect)
{
    camera->aspect = aspect;
    setProjection(camera);
}

void setCamFOV(Camera *camera, float fov)
{
    if (camera->projectionType == PERSPECTIVE)
        perspective(&camera->projection, fov, camera->aspect, 0.1f, 100.0f);
}