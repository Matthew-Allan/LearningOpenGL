#ifndef FILES_H
#define FLIES_H

#include <stdio.h>

size_t getFileSize(FILE *file_pointer);
void *readFile(char *filepath);

#endif