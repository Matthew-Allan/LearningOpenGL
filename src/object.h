#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include "transforms.h"

typedef struct{
    GLuint vao;
    GLuint vertexCount;
    GLuint textures[32];
    int textureCount;
    vec3 pos;
} WorldObject;

WorldObject *createWorldObject(GLuint object, GLuint vertexCount);
void addTexToWorldObject(WorldObject *object, GLuint texture, char *uniformName, GLuint shaderProgram);

#endif