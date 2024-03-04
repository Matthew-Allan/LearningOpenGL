#include "object.h"

#include <stdlib.h>

WorldObject *createWorldObject(GLuint VAO, GLuint vertexCount, vec3 pos, uint64_t id)
{
    WorldObject *object = (WorldObject *)malloc(sizeof(WorldObject));
    object->vao = VAO;
    object->vertexCount = vertexCount;
    object->textureCount = 0;
    object->pos = pos;
    object->next = NULL;
    object->id = id;
    return object;
}

void addTexToWorldObject(WorldObject *object, GLuint texture, char *uniformName, GLuint shaderProgram)
{
    object->textures[object->textureCount] = texture;
    
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName), object->textureCount);
    object->textureCount++;
}

WorldObject *freeObject(WorldObject *object)
{
    WorldObject *next = object->next;
    free(object);
    return next;
}

WorldObject *freeObjects(WorldObject *objects, ssize_t count)
{
    WorldObject *object = objects;
    for(; count != 0 && object != NULL; count -= count > 0)
        object = freeObject(object);
    return object;
}