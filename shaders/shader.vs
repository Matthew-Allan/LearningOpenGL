#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 TexCoord;
uniform int ticks;

void main()
{
    float theta = ticks / 1000.0;
    vertexColor = aColor;
    float sum = aPos.y + aPos.x;
    vec3 rotate = aPos;
    //rotate = mat3(vec3(1, 0, 0), vec3(0, cos(theta), -sin(theta)) ,vec3(0, sin(theta), cos(theta))) * rotate;
    //rotate = mat3(vec3(cos(theta), 0, sin(theta)), vec3(0, 1, 0) ,vec3(-sin(theta), 0, cos(theta))) * rotate;
    rotate = mat3(vec3(cos(theta), -sin(theta), 0), vec3(sin(theta), cos(theta), 0) ,vec3(0, 0, 1)) * rotate;
    gl_Position = vec4(rotate, 1.0);
    //gl_Position = vec4(rotate.x + sin(theta + sum) / 20, rotate.y + cos(theta + sum) / 20, rotate.z, 1.0);
    TexCoord = aTexCoord;
}