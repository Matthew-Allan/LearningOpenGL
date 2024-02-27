#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "app.h"
#include "files.h"

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

char *getPath()
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
    }
    char * path = (char *)malloc(sizeof(char) * pathlen);
    GET_PATH(path, pathlen);
    char *endOfDir = strrchr(path, PATH_SEPERATOR) + 1;
    *endOfDir = '\0';
    return path;
}

App *setUpApp(SDL_Window *window)
{
    printf("Initialising App\n");
    
    App *app = (App *)malloc(sizeof(App));

    if (app == NULL)
    {
        printf("Couldn't allocate space for app");
        return NULL;
    }

    app->running = true;
    app->window = window;
    app->images = NULL;
    app->path = getPath();

    printf("App initialised\n");

    return app;
}

void closeAppWindow(App *app)
{
    if (app->window == NULL)
        return;
    SDL_DestroyWindow(app->window);
    app->window = NULL;
}

int closeApp(App *app, int code)
{
    closeAppWindow(app);
    free(app->path);
    freeImages(app->images, &app->images, -1);
    free(app);
    SDL_Quit();
    printf("Goodbye!");
    return code;
}

void *readResource(char *relativePath, App *app)
{
    GET_ABS_PATH(absolutePath, relativePath, app->path);
    return readFile(absolutePath);
}

Image *readImageRsrc(char *relativePath, App *app, bool flipped)
{
    GET_ABS_PATH(absolutePath, relativePath, app->path);
    Image * image = loadImage(absolutePath, flipped);
    image->next = app->images;
    app->images = image;
    return image;
}