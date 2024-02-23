#version 460 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec4 forground = texture(texture0, TexCoord);
    vec4 background = texture(texture1, TexCoord);
    FragColor = forground * (1.0 - background.a) + background;
} 