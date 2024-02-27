#include <stdlib.h>

#include "buffers.h"

GLuint createEBO(GLuint indices[], size_t verticesSize)
{
    GLuint EBO;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesSize, indices, GL_STATIC_DRAW);

    return EBO;
}

GLuint createVBO(void *vertices, size_t verticesSize, VertexAttribute *vertexAttrs, size_t vertexAttrCount)
{
    GLuint VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    for (int i = 0; i < vertexAttrCount; i++)
    {
        VertexAttribute attr = vertexAttrs[i];
        glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalised, attr.stride, attr.pointer);
        glEnableVertexAttribArray(attr.index);
    }

    return VBO;
}

GLuint createVAO(void *vertices, size_t verticesSize, GLuint indices[], size_t indicesSize, VertexAttribute *vertexAttrs, size_t vertexAttrCount)
{
    GLuint VAO;

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    if (verticesSize > 0)
        createVBO(vertices, verticesSize, vertexAttrs, vertexAttrCount);
    if (indicesSize > 0)
        createEBO(indices, indicesSize);

    return VAO;
}

VertexAttributeObject *createVAOStruct(GLuint VAO, GLuint vertexCount)
{
    VertexAttributeObject *VAOStruct = (VertexAttributeObject *)malloc(sizeof(VertexAttributeObject));
    VAOStruct->vao = VAO;
    VAOStruct->count = vertexCount;
    VAOStruct->textureCount = 0;
    return VAOStruct;
}

void addTexToVAO(VertexAttributeObject *VAO, GLuint texture, char *uniformName, GLuint shaderProgram)
{
    VAO->textures[VAO->textureCount] = texture;
    
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, uniformName), VAO->textureCount);
    VAO->textureCount++;
}