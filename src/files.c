#include <stdlib.h>

#include "files.h"

size_t getFileSize(FILE *file_pointer)
{
    size_t size = 0;
    long prev_pos = ftell(file_pointer);
    for (char ch = fgetc(file_pointer); ch != EOF; ch = fgetc(file_pointer))
        size++;
    fseek(file_pointer, prev_pos, SEEK_SET);
    return size;
}

void *readFile(char *filepath)
{
    FILE *file_pointer;
    if ((file_pointer = fopen(filepath, "r")) == NULL)
    {
        printf("Failed to load \"%s\".", filepath);
        return NULL;
    }

    size_t file_size = getFileSize(file_pointer);
    void *contents = (char *)malloc(file_size + 1);
    if (contents == NULL)
    {
        printf("Failed to load \"%s\". No space.", filepath);
        fclose(file_pointer);
        return NULL;
    }

    fread(contents, 1, file_size, file_pointer);
    ((char *)contents)[file_size] = '\0';

    fclose(file_pointer);

    return contents;
}