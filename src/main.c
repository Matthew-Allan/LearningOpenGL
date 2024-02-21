#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "shader.h"

#ifdef _WIN32
#include <windows.h>
#define GET_PATH(out, len) GetModuleFileName(NULL, out, len)
#define PATH_SEPERATOR '\\'

#elif __linux__
#include <unistd.h>
#define GET_PATH(out, len) readlink("/proc/self/exe", out, len);
#define PATH_SEPERATOR '/'

#endif

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 640
#define SCREEN_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

#define GLAD_MAJOR_VERSION 4
#define GLAD_MINOR_VERSION 6
#define GLAD_PROFILE_MASK SDL_GL_CONTEXT_PROFILE_CORE

#define VSYNC 1
#define POLYGON_MODE GL_LINE

#define GET_MILLIS(timer) (1000 * (timer).time + (timer).millitm)

#define MAX()

#define CAT_STRINGS(a, b, dest) (strcat(strcat((dest), (a)), (b)))

typedef struct
{
    bool running;
    SDL_Window *window;
    char *path;
} App;

void getPath(App *app)
{
    size_t pathlen = 5;
    size_t outputlen = pathlen;
    while (outputlen == pathlen)
    {
        pathlen += 10;
        char test[pathlen];
        outputlen = GET_PATH(test, pathlen);
    }
    app->path = (char *)malloc(sizeof(char) * pathlen);
    GET_PATH(app->path, pathlen);
    char *endOfDir = strrchr(app->path, PATH_SEPERATOR);
    *endOfDir = '\0';
}

App *setUpApp()
{
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not init. Error: %s\n", SDL_GetError());
        return NULL;
    }

    int errorCheck = 0;
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GLAD_MAJOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GLAD_MINOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, GLAD_PROFILE_MASK);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, true);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (errorCheck < 0)
    {
        printf("Setting attributes failed. Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Window *window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SCREEN_FLAGS);

    if (window == NULL)
    {
        printf("Window could not be created. Error: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    App *app = (App *)malloc(sizeof(App));

    if (app == NULL)
    {
        printf("Couldn't allocate space for app");
        return NULL;
    }
    if (SDL_GL_SetSwapInterval(VSYNC) != 0)
    {
        printf("Could not enable vsync. Error: %s\n", SDL_GetError());
        return NULL;
    }

    getPath(app);

    app->running = true;
    app->window = window;

    return app;
}

GLuint loadShader(char *relativePath, App *app, int type)
{
    char absolutePath[strlen(app->path) + strlen(relativePath)];
    absolutePath[0] = '\0';

    return compileShader(CAT_STRINGS(app->path, relativePath, absolutePath), type);
}

GLuint createProgram(App *app, GLuint shaders[], size_t shaderCount)
{
    GLuint shaderProgram = glCreateProgram();

    for(int i = 0; i < shaderCount; i++)
        glAttachShader(shaderProgram, shaders[i]);

    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader linking failed: Error %s\n", infoLog);
        glDeleteProgram(shaderProgram);
        return 0;
    }

    return shaderProgram;
}

void closeApp(App *app)
{
    SDL_DestroyWindow(app->window);
    free(app->path);
    free(app);
    SDL_Quit();
    printf("Goodbye!");
}

void resizeViewport(SDL_Window *window)
{
    int width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

GLuint createEBO(GLuint indices[], size_t verticesSize)
{
    GLuint EBO;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verticesSize, indices, GL_STATIC_DRAW); 

    return EBO;
}

GLuint createVBO(float vertices[], size_t size)
{
    GLuint VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return VBO;
}

GLuint createVAO(float vertices[], size_t verticesSize, GLuint indices[], size_t indicesSize)
{
    GLuint VAO;

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    if(verticesSize > 0)
        createVBO(vertices, verticesSize);
    if(indicesSize > 0)
        createEBO(indices, indicesSize);

    return VAO;
}

void pollEvents(App *app)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        // If the switch statement isn't the most truly ugly bit of syntax, I dont know what is.
        switch (e.type)
        {
        case SDL_QUIT:
            app->running = false;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                resizeViewport(app->window);
            break;
        default:
            break;
        }
    }
}

void drawCall(App *app, GLuint shaderProgram, GLuint *VAO, GLuint *vertexCounts, size_t VAOCount)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint64 time = SDL_GetTicks64();
    float greenValue = (sin(time / 1000.0) / 2.0f) + 0.5f;

    int vertexColorLocation = glGetUniformLocation(shaderProgram, "color");
    glUseProgram(shaderProgram);
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    for (int i = 0; i < VAOCount; i++)
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[i]);
        glDrawElements(GL_TRIANGLES, vertexCounts[i], GL_UNSIGNED_INT, 0);
    }
}

int main(int argc, char *argv[])
{
    App *app;

    if ((app = setUpApp()) == NULL)
    {
        SDL_DestroyWindow(app->window);
        return 1;
    }

    GLuint shaders[2];
    size_t shaderCount = 2;

    if ((shaders[0] = loadShader("\\shaders\\shader.vs", app, GL_VERTEX_SHADER)) == 0)
        return 1;
    if ((shaders[1] = loadShader("\\shaders\\shader.fs", app, GL_FRAGMENT_SHADER)) == 0)
        return 1;

    GLuint shaderProgram;
    if ((shaderProgram = createProgram(app, shaders, shaderCount)) == 0)
    {
        closeApp(app);
        return 1;
    }

    for(int i = 0; i < shaderCount; i++)
        glDeleteShader(shaders[i]);

    GLuint VAOs[1];
    GLuint vertexCounts[1];

    // VAO 0

    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f};

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};

    VAOs[0] = createVAO(vertices, sizeof(vertices), indices, sizeof(indices));
    vertexCounts[0] = 6;

    glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE);

    while (app->running)
    {
        // Poll events such as resizing window, hitting keys, or exiting the app.
        pollEvents(app);

        // Draw to screen
        drawCall(app, shaderProgram, VAOs, vertexCounts, 1);

        // Flip buffers.
        SDL_GL_SwapWindow(app->window);
    }

    closeApp(app);

    return 0;
}
