#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 TexCoord;
uniform int ticks;

void main()
{
    vertexColor = aColor;
    float sum = aPos.y + aPos.x;
    gl_Position = vec4(aPos.x + sin((ticks / 1000.0) + sum) / 20, aPos.y + cos((ticks / 1000.0) + sum) / 20, aPos.z, 1.0);
    TexCoord = aTexCoord;
}