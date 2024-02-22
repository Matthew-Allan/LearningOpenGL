#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
out vec4 vertexColor;
uniform int ticks;

void main()
{
    vertexColor = vec4(aColor, 1.0);
    float sum = aPos.y + aPos.x;
    gl_Position = vec4(aPos.x + sin((ticks / 1000.0) + sum) / 20, aPos.y + cos((ticks / 1000.0) + sum) / 20, aPos.z, 1.0);
}