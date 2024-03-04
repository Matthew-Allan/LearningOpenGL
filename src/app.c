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
    app->keyboardState = SDL_GetKeyboardState(NULL);
    app->axes = NULL;
    app->prevTime = SDL_GetTicks64();
    app->deltaTime = 0;
    app->prevSec = SDL_GetTicks64();
    app->frames = 0;
    app->mouseXDelta = 0;
    app->mouseYDelta = 0;
    app->scrollDelta = 0;

    updateDimentions(app);

    SDL_WarpMouseInWindow(app->window, app->w / 2, app->h / 2);

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

void closeApp(App *app)
{
    closeAppWindow(app);
    free(app->path);
    freeImages(app->images, -1);
    freeAxex(app->axes, -1);
    free(app);
    SDL_Quit();
    printf("Goodbye!");
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

void updateDimentions(App *app)
{
    SDL_GL_GetDrawableSize(app->window, &app->w, &app->h);
}

void tickFrame(App *app)
{
    Uint64 ticks = SDL_GetTicks64();
    app->deltaTime = ticks - app->prevTime;
    app->prevTime = ticks;
    app->frames++;
    if(app->prevSec - ticks > 100)
    {
        app->fps = app->frames * 10;
        app->frames = 0;
        app->prevSec += 100;
    }
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    SDL_WarpMouseInWindow(app->window, app->w / 2, app->h / 2);
    app->mouseXDelta = mouseX - app->w / 2;
    app->mouseYDelta = mouseY - app->h / 2;
    app->scrollDelta = 0;
}

int addAxis(App *app, Axis *axis)
{
    for(Axis *existingAxis = app->axes; existingAxis != NULL; existingAxis = existingAxis->next)
        if(!strcmp(axis->name, existingAxis->name))
            return 1;
    axis->next = app->axes;
    app->axes = axis;
    return 0;
}

Axis *getAxis(App *app, char *name)
{
    for(Axis *axis = app->axes; axis != NULL; axis = axis->next)
        if(!strcmp(axis->name, name))
            return axis;
    return NULL;
}

int getAxisValue(App *app, char *name)
{
    int value = 0;
    Axis *axis = getAxis(app, name);
    for(int i = 0; i < axis->positiveCount; i++)
        value += app->keyboardState[axis->positive[i]];
    for(int i = 0; i < axis->negativeCount; i++)
        value -= app->keyboardState[axis->negative[i]];
    return (value == 0) ? 0 : value / abs(value);
}