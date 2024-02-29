#version 410 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform int ticks;

vec2 getRipple()
{
    float offset = ticks / 8000.0;
    vec2 ripplea = texture(texture2, TexCoord + vec2(offset, offset)).xy / 40;
    vec2 rippleb = texture(texture2, TexCoord - vec2(offset, offset)).xy / 40;
    return ripplea - rippleb;
}

vec4 getBright(vec2 coord)
{
    float offset = ticks / 32000.0;
    float causticsa = texture(texture3, (coord * 2) + vec2(offset * 1, offset * 4.5)).r;
    float causticsb = texture(texture3, (coord * -2) + vec2(offset * 2, offset * 3)).r;
    float bright = max(causticsa, causticsb) + 0.5;
    return vec4(bright, bright, bright, 1); 
}

vec4 getImage(vec2 coord)
{
    vec4 forground = texture(texture0, vec2(coord.x, 1 - coord.y));
    vec4 background = texture(texture1, coord);
    vec4 image = (forground * (1.0 - background.a)) + (background * background.a);
    image = vec4(vertexColor,  1.0) * (1.0 - image.a) + (image * image.a);
    return image * getBright(coord);
}

void main()
{
    FragColor = getImage(TexCoord + getRipple());
} 