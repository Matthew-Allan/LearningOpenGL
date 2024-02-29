#include "object.h"

#include <stdlib.h>

WorldObject *createWorldObject(GLuint VAO, GLuint vertexCount)
{
    WorldObject *object = (WorldObject *)malloc(sizeof(WorldObject));
    object->vao = VAO;
    object->vertexCount = vertexCount;
    object->textureCount = 0;
    object->pos = vec3(0, 0, 0);
    return object;
}

void addTexToWorldObject(WorldObject *object, GLuint texture, char *uniformName, GLuint shaderProgram)
{
    object->textures[object->textureCount] = texture;
    
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName), object->textureCount);
    object->textureCount++;
}