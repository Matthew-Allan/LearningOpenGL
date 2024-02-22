#include <stdlib.h>
#include <stdio.h>

#include "app.h"

#ifdef _WIN32
#include <windows.h>
#define GET_PATH(out, len) GetModuleFileName(NULL, out, len)
#define PATH_SEPERATOR '\\'
#endif

// Linux remains untested
#ifdef __linux__
#include <unistd.h>
#define GET_PATH(out, len) readlink("/proc/self/exe", out, len);
#define PATH_SEPERATOR '/'
#endif

void getPath(char **out)
{
    size_t pathlen = 5;
    size_t outputlen = pathlen;
    while (outputlen == pathlen)
    {
        pathlen += 10;
        char test[pathlen];
        outputlen = GET_PATH(test, pathlen);
    }
    *out = (char *)malloc(sizeof(char) * pathlen);
    GET_PATH(*out, pathlen);
    char *endOfDir = strrchr(*out, PATH_SEPERATOR);
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