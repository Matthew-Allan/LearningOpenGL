#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "shader.h"
#include "app.h"
#include "buffers.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 640
#define SCREEN_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

#define GLAD_MAJOR_VERSION 4
#define GLAD_MINOR_VERSION 6
#define GLAD_PROFILE_MASK SDL_GL_CONTEXT_PROFILE_CORE

#define POLYGON_MODE GL_FILL

#define VSYNC 1

int setGLAttributes()
{
    int errorCheck = 0;
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GLAD_MAJOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GLAD_MINOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, GLAD_PROFILE_MASK);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, true);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    return errorCheck;
}

SDL_Window *setUpWindow()
{
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");

    printf("Initialising SLD\n");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0)
    {
        printf("SDL could not init. Error: %s\n", SDL_GetError());
        return NULL;
    }

    printf("Setting GL attributes\n");
    if (setGLAttributes() < 0)
    {
        printf("Setting attributes failed. Error: %s\n", SDL_GetError());
        return NULL;
    }

    printf("Creating Window\n");
    SDL_Window *window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SCREEN_FLAGS);

    if (window == NULL)
    {
        printf("Window could not be created. Error: %s\n", SDL_GetError());
        return NULL;
    }

    printf("Creating OpenGL context\n");
    SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        SDL_DestroyWindow(window);
        return NULL;
    }

    if (SDL_GL_SetSwapInterval(VSYNC) != 0)
    {
        printf("Could not enable vsync. Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return NULL;
    }

    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    return window;
}

void resizeViewport(SDL_Window *window)
{
    int width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    glViewport(0, 0, width, height);
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

void draw(App *app, GLuint shaderProgram, GLuint *VAO, GLuint *vertexCounts, size_t VAOCount)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < VAOCount; i++)
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[i]);
        glDrawElements(GL_TRIANGLES, vertexCounts[i], GL_UNSIGNED_INT, 0);
    }
}

int main(int argc, char *argv[])
{
    printf("Initialising SDL window\n");
    SDL_Window *window;

    if ((window = setUpWindow()) == NULL)
        return 1;

    printf("Initialising App\n");
    App *app;

    if ((app = setUpApp(window)) == NULL)
    {
        SDL_DestroyWindow(window);
        return 1;
    }

    GLuint shaders[2];
    size_t shaderCount = 2;

    printf("Loading/Compiling shaders\\shader.vs\n");
    if ((shaders[0] = loadShader("/shaders/shader.vs", app, GL_VERTEX_SHADER)) == 0)
        return closeApp(app, 1);
    printf("Loading/Compiling shaders\\shader.fs\n");
    if ((shaders[1] = loadShader("/shaders/shader.fs", app, GL_FRAGMENT_SHADER)) == 0)
        return closeApp(app, 1);

    printf("Creating shader program\n");
    GLuint shaderProgram;
    if ((shaderProgram = createProgram(app, shaders, shaderCount)) == 0)
        return closeApp(app, 1);

    for (int i = 0; i < shaderCount; i++)
        glDeleteShader(shaders[i]);

    GLuint VAOs[1];
    GLuint vertexCounts[1];

    // VAO 0

    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};

    VertexAttribute vertexAttributes[2];
    vertexAttributes[0] = (VertexAttribute){0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0};
    vertexAttributes[1] = (VertexAttribute){1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float))};

    printf("Creating VAO\n");
    VAOs[0] = createVAO(vertices, sizeof(vertices), indices, sizeof(indices), vertexAttributes, 2);
    vertexCounts[0] = 6;

    glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE);

    printf("Done!\n");

    while (app->running)
    {
        // Poll events such as resizing window, hitting keys, or exiting the app.
        pollEvents(app);

        // Draw to screen
        draw(app, shaderProgram, VAOs, vertexCounts, 1);

        // Flip buffers.
        SDL_GL_SwapWindow(app->window);
    }

    return closeApp(app, 0);
}
