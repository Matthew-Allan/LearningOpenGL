#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>

#include "shader.h"

size_t getFileSize(FILE *file_pointer)
{
    size_t size = 0;
    long prev_pos = ftell(file_pointer);
    for(char ch = fgetc(file_pointer); ch != EOF; ch = fgetc(file_pointer))
        size++;
    fseek(file_pointer, prev_pos, SEEK_SET);
    return size;
}

char *readFile(char *filepath)
{
    FILE *file_pointer;
    if ((file_pointer = fopen(filepath, "r")) == NULL)
        return NULL;

    size_t file_size = getFileSize(file_pointer);
    char *contents = (char *)malloc(sizeof(char) * (file_size + 1));

    fread(contents, 1, file_size, file_pointer);
    contents[file_size] = '\0';

    fclose(file_pointer);

    return contents;
}

GLuint compileShader(char *filepath, int type)
{
    const char *source;
    if((source = readFile(filepath)) == NULL)
    {
        printf("Failed to load shader file. (\"%s\")\n", filepath);
        return 0;
    }

    printf("%s\n", source);
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed (\"%s\"): Error %s\n", filepath, infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
