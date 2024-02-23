#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "image.h"
#include "app.h"

#ifdef _WIN32
#include <windows.h>
#define GET_PATH(out, len) GetModuleFileName(NULL, out, len)
#define PATH_SEPERATOR '\\'
#endif

#ifdef __linux__
#include <unistd.h>
#define GET_PATH(out, len) readlink("/proc/self/exe", out, len);
#define PATH_SEPERATOR '/'
#endif

#ifdef __APPLE__
#include <mach-o/dyld.h>
#define GET_PATH(out, len) _NSGetExecutablePath(out, (uint32_t *)&(len));
#define PATH_SEPERATOR '/'
#endif

#define CAT_STRINGS(a, b, dest) (strcat(strcat((dest), (a)), (b)))
#define GET_ABS_PATH(name, relPath, appPath) char (name)[strlen(appPath) + strlen(relPath) + 1]; (name)[0] = '\0'; CAT_STRINGS(appPath, relPath, name)

void getPath(char **out)
{
    size_t pathlen = 5;
    ssize_t outputlen = pathlen;
    while (outputlen == pathlen)
    {
        pathlen += 10;
        char test[pathlen];
        outputlen = GET_PATH(test, pathlen);
        if(outputlen < 0)
            outputlen = pathlen;
        printf("nuts %s\n", test);
    }
    *out = (char *)malloc(sizeof(char) * pathlen);
    GET_PATH(*out, pathlen);
    char *endOfDir = strrchr(*out, PATH_SEPERATOR) + 1;
    *endOfDir = '\0';
}

App *setUpApp(SDL_Window *window)
{
    App *app = (App *)malloc(sizeof(App));

    if (app == NULL)
    {
        printf("Couldn't allocate space for app");
        return NULL;
    }

    app->running = true;
    app->window = window;
    getPath(&app->path);

    printf("App initialised\n");

    return app;
}

int closeApp(App *app, int code)
{
    SDL_DestroyWindow(app->window);
    free(app->path);
    free(app);
    SDL_Quit();
    printf("Goodbye!");
    return code;
}

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

void *readResource(char *relativePath, App *app)
{
    GET_ABS_PATH(absolutePath, relativePath, app->path);
    return readFile(absolutePath);
}

uint8_t *readImageRsrc(char *relativePath, App *app, size_t *width, size_t *height, int *nrChannels)
{
    GET_ABS_PATH(absolutePath, relativePath, app->path);
    return loadImage(absolutePath, width, height, nrChannels, true);
}