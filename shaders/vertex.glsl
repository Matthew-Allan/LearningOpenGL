#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

vec3 wiggle(vec3 vector, float time)
{
    float sum = vector.y + vector.x;
    return vec3(vector.x + sin(time + sum) / 20, vector.y + cos(time + sum) / 20, vector.z);
}

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    vertexColor = aColor;
}