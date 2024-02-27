#ifndef BUFFERS_H
#define BUFFERS_H

#include <glad/glad.h>

typedef struct
{
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalised;
    GLsizei stride;
    GLvoid *pointer;
} VertexAttribute;

typedef struct{
    GLuint vao;
    GLuint count;
    GLuint textures[32];
    int textureCount;
} VertexAttributeObject;

GLuint createEBO(GLuint indices[], size_t verticesSize);
GLuint createVBO(void *vertices, size_t verticesSize, VertexAttribute *vertexAttrs, size_t vertexAttrCount);
GLuint createVAO(void *vertices, size_t verticesSize, GLuint indices[], size_t indicesSize, VertexAttribute *vertexAttrs, size_t vertexAttrCount);
void addTexToVAO(VertexAttributeObject *VAO, GLuint texture, char *uniformName, GLuint shaderProgram);

#endif