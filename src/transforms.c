#include "transforms.h"

#include <math.h>
#include <string.h>

#define SQR(val) (val * val)

float mag2sqr(vec2 vector)
{
    return SQR(vector.x) + SQR(vector.y);
}

float mag3sqr(vec3 vector)
{
    return SQR(vector.x) + SQR(vector.y) + SQR(vector.z);
}

float mag4sqr(vec4 vector)
{
    return SQR(vector.x) + SQR(vector.y) + SQR(vector.z) + SQR(vector.w);
}

float mag2(vec2 vector)
{
    return sqrtf(mag2sqr(vector));
}

float mag3(vec3 vector)
{
    return sqrtf(mag3sqr(vector));
}

float mag4(vec4 vector)
{
    return sqrtf(mag4sqr(vector));
}

void norm2(vec2 in, vec2 *out)
{
    float scale = 1.0f / mag2(in);
    multiply2(scaleMat2(scale, scale), &in, out, 1);
}

void norm3(vec3 in, vec3 *out)
{
    float scale = 1.0f / mag3(in);
    multiply3(scaleMat3(scale, scale, scale), &in, out, 1);
}

void norm4(vec4 in, vec4 *out)
{
    float scale = 1.0f / mag4(in);
    multiply4(scaleMat4(scale, scale, scale, scale), &in, out, 1);
}

void norm4Homogeneous(vec4 in, vec4 *out)
{
    float scale = 1.0f / mag3(vec3(in.x, in.y, in.z));
    multiply4(scaleMat4(scale, scale, scale, 1), &in, out, 1);
}

float dot2(vec2 inA, vec2 inB)
{
    return (inA.x * inB.x) + (inA.y * inB.y);
}

float dot3(vec3 inA, vec3 inB)
{
    return (inA.x * inB.x) + (inA.y * inB.y) + (inA.z * inB.z);
}

float dot4(vec4 inA, vec4 inB)
{
    return (inA.x * inB.x) + (inA.y * inB.y) + (inA.z * inB.z) + (inA.w * inB.w);
}

void multiply2(mat2 inA, vec2 *inB, vec2 *out, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        vec2 vector = inB[i];
        out[i].x = dot2(vec2(inA.i.x, inA.j.x), vector);
        out[i].y = dot2(vec2(inA.i.y, inA.j.y), vector);
    }
}

void multiply3(mat3 inA, vec3 *inB, vec3 *out, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        vec3 vector = inB[i];
        out[i].x = dot3(vec3(inA.i.x, inA.j.x, inA.k.x), vector);
        out[i].y = dot3(vec3(inA.i.y, inA.j.y, inA.k.y), vector);
        out[i].z = dot3(vec3(inA.i.z, inA.j.z, inA.k.z), vector);
    }
}

void multiply4(mat4 inA, vec4 *inB, vec4 *out, size_t size)
{
    for(size_t i = 0; i < size; i++)
    {
        vec4 vector = inB[i];
        out[i].x = dot4(vec4(inA.i.x, inA.j.x, inA.k.x, inA.l.x), vector);
        out[i].y = dot4(vec4(inA.i.y, inA.j.y, inA.k.y, inA.l.y), vector);
        out[i].z = dot4(vec4(inA.i.z, inA.j.z, inA.k.z, inA.l.z), vector);
        out[i].w = dot4(vec4(inA.i.w, inA.j.w, inA.k.w, inA.l.w), vector);
    }
}

void genRotationMatrix(mat4 *out, float angle, vec3 axis)
{
    float sinval = sinf(angle);
    float cosval = cosf(angle);

    vec3 normAxis;
    norm3(axis, &normAxis);

    out->i = vec4(cosval + (SQR(normAxis.x) * (1 - cosval)), (normAxis.y * normAxis.x * (1 - cosval)) + (normAxis.z * sinval), (normAxis.z * normAxis.x * (1 - cosval)) - (normAxis.y * sinval), 0);
    out->j = vec4((normAxis.x * normAxis.y * (1 - cosval)) - (normAxis.z * sinval), cosval + (SQR(normAxis.y) * (1 - cosval)), (normAxis.z * normAxis.y * (1 - cosval)) + (normAxis.x * sinval), 0);
    out->k = vec4((normAxis.x * normAxis.z * (1 - cosval)) + (normAxis.y * sinval), (normAxis.y * normAxis.z * (1 - cosval)) - (normAxis.x * sinval), cosval + (SQR(normAxis.z) * (1 - cosval)), 0);
    out->l = vec4(0, 0, 0, 1);
}

void rotate(vec4 *in, vec4 *out, size_t size, float angle, vec3 axis)
{
    mat4 rotationMatrix;
    genRotationMatrix(&rotationMatrix, angle, axis);
    multiply4(rotationMatrix, in, out, size);
}

void translate(vec4 *in, vec4 *out, size_t size, vec3 trans)
{
    multiply4(translationMat4(trans.x, trans.y, trans.z), in, out, size);
}

void scale(vec4 *in, vec4 *out, size_t size, vec3 scale)
{
    multiply4(scaleMat4(scale.x, scale.y, scale.z, 1), in, out, size);
}

void orthographic(mat4 *out, float left, float right, float bottom, float top, float near, float far)
{
    out->i = vec4(2.0f / (right - left), 0, 0, 0);
    out->j = vec4(0, 2.0f / (top - bottom), 0, 0);
    out->k = vec4(0, 0, -2.0f / (far - near), 0);
    out->l = vec4(-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1);
}

void perspective(mat4 *out, float fov, float aspect, float near, float far)
{
    float f = cos(fov / 2) / sin(fov / 2);
    out->i = vec4(f / aspect, 0, 0, 0);
    out->j = vec4(0, f, 0, 0);
    out->k = vec4(0, 0, (far + near) / (near - far), -1);
    out->l = vec4(0, 0, 2.0f * far * near / (near - far), 0);
}