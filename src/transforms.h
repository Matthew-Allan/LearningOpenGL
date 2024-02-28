#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <sys/types.h>

typedef struct
{
    float x;
    float y;
} vec2;

typedef struct
{
    float x;
    float y;
    float z;
} vec3;

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec4;

typedef struct
{
    vec2 i;
    vec2 j;
} mat2;

typedef struct
{
    vec3 i;
    vec3 j;
    vec3 k;
} mat3;

typedef struct
{
    vec4 i;
    vec4 j;
    vec4 k;
    vec4 l;
} mat4;

#define scaleMat2(x, y) ((mat2){(x), 0, 0, (y)})
#define scaleMat3(x, y, z) ((mat3){(x), 0, 0, 0, (y), 0, 0, 0, (z)})
#define scaleMat4(x, y, z, w) ((mat4){(x), 0, 0, 0, 0, (y), 0, 0, 0, 0, (z), 0, 0, 0, 0, (w)})

#define identMat2 scaleMat2(1, 1)
#define identMat3 scaleMat3(1, 1, 1)
#define identMat4 scaleMat4(1, 1, 1, 1)

#define translationMat3(x, y) ((mat3){1, 0, 0, 0, 1, 0, (x), (y), 1})
#define translationMat4(x, y, z) ((mat4){1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, (x), (y), (z), 1})

#define emptyMat4(w) scaleMat4(0, 0, 0, 1)

float mag2sqr(vec2 vector);
float mag3sqr(vec3 vector);
float mag4sqr(vec4 vector);

float mag2(vec2 vector);
float mag3(vec3 vector);
float mag4(vec4 vector);

void norm2(vec2 in, vec2 *out);
void norm3(vec3 in, vec3 *out);
void norm4(vec4 in, vec4 *out);

void norm4Homogeneous(vec4 in, vec4 *out);

float dot2(vec2 inA, vec2 inB);
float dot3(vec3 inA, vec3 inB);
float dot4(vec4 inA, vec4 inB);

void multiply2(mat2 inA, vec2 *inB, vec2 *out, size_t size);
void multiply3(mat3 inA, vec3 *inB, vec3 *out, size_t size);
void multiply4(mat4 inA, vec4 *inB, vec4 *out, size_t size);

void genRotationMatrix(mat4 *out, float angle, vec3 axis);

void rotate(vec4 *in, vec4 *out, size_t size, float angle, vec3 axis);

#endif