#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 TexCoord;
uniform int ticks;
uniform mat4 rotationTrans;

vec3 wiggle(vec3 vector, float time)
{
    float sum = vector.y + vector.x;
    return vec3(vector.x + sin(time + sum) / 20, vector.y + cos(time + sum) / 20, vector.z);
}

void main()
{
    float seconds = ticks / 1000.0;
    vec3 pos = aPos;
    //pos = wiggle(aPos, seconds);
    gl_Position = rotationTrans * vec4(pos, 1.0);
    TexCoord = aTexCoord;
    vertexColor = aColor;
}