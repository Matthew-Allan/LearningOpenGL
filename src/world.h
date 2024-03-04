#ifndef WORLD_H
#define WORLD_H

#include "camera.h"
#include "object.h"

typedef struct
{
    Camera *camera;
    WorldObject *objects;
    uint64_t prevId;
} World;

World *createWorld();
void setCamera(World *world, Camera *camera);
void addWorldObject(World *world, WorldObject *object);
void freeWorld(World *world);

#endif