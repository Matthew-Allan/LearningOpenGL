#include <stdlib.h>

#include "world.h"

World *createWorld()
{
    World *world = (World *)malloc(sizeof(World));
    world->camera = NULL;
    world->objects = NULL;
    world->prevId = 0;
    return world;
}

void setCamera(World *world, Camera *camera)
{
    world->camera = camera;
}

void addWorldObject(World *world, WorldObject *object)
{
    object->next = world->objects;
    world->objects = object;
}

void freeWorld(World *world)
{
    freeObjects(world->objects, -1);
    free(world->camera);
    free(world);
}