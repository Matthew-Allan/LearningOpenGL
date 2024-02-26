#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 TexCoord;
uniform int ticks;

vec3 rotatex(vec3 vector, float angle)
{
    float cosval = cos(angle);
    float sinval = sin(angle);
    return mat3(vec3(1, 0, 0), vec3(0, cosval, -sinval) ,vec3(0, sinval, cosval)) * vector;
}

vec3 rotatey(vec3 vector, float angle)
{
    float cosval = cos(angle);
    float sinval = sin(angle);
    return mat3(vec3(cosval, 0, sinval), vec3(0, 1, 0) ,vec3(-sinval, 0, cosval)) * vector;
}

vec3 rotatez(vec3 vector, float angle)
{
    float cosval = cos(angle);
    float sinval = sin(angle);
    return mat3(vec3(cosval, -sinval, 0), vec3(sinval, cosval, 0) ,vec3(0, 0, 1)) * vector;
}

vec3 wiggle(vec3 vector, float time)
{
    float sum = vector.y + vector.x;
    return vec3(vector.x + sin(time + sum) / 20, vector.y + cos(time + sum) / 20, vector.z);
}

void main()
{
    float seconds = ticks / 1000.0;
    vec3 rotate = wiggle(aPos, seconds);
    //rotate = rotatex(rotate, seconds / 10);
    //rotate = rotatey(rotate, seconds / 10);
    //rotate = rotatez(rotate, seconds / 10);
    gl_Position = vec4(rotate, 1.0);
    TexCoord = aTexCoord;
    vertexColor = aColor;
}