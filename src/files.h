#ifndef FILES_H
#define FILES_H

#include <stdio.h>

size_t getFileSize(FILE *file_pointer);
void *readFile(char *filepath);

#endif