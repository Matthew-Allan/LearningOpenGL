#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include "transforms.h"

typedef struct WorldObject{
    GLuint vao;
    GLuint vertexCount;
    GLuint textures[32];
    int textureCount;
    vec3 pos;
    uint64_t id;
    struct WorldObject *next;
} WorldObject;

WorldObject *createWorldObject(GLuint VAO, GLuint vertexCount, vec3 pos, uint64_t id);
void addTexToWorldObject(WorldObject *object, GLuint texture, char *uniformName, GLuint shaderProgram);
WorldObject *freeObject(WorldObject *object);
WorldObject *freeObjects(WorldObject *objects, ssize_t count);

#endif