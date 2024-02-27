#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "shader.h"
#include "app.h"
#include "buffers.h"
#include "image.h"
#include "texture.h"

#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 640
#define SCREEN_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

#define GLAD_MAJOR_VERSION 4
#define GLAD_MINOR_VERSION 1
#define GLAD_PROFILE_MASK SDL_GL_CONTEXT_PROFILE_CORE

#define POLYGON_MODE GL_FILL

#define VSYNC 1

void resizeViewport(SDL_Window *window)
{
    int width, height;
    SDL_GL_GetDrawableSize(window, &width, &height);
    glViewport(0, 0, width, height);
}

int setGLAttributes()
{
    int errorCheck = 0;
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GLAD_MAJOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GLAD_MINOR_VERSION);
    errorCheck += SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, GLAD_PROFILE_MASK);
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

    printf("Initialising SLD_image\n");
    if (init_image() < 0)
    {
        SDL_Quit();
        return NULL;
    }

    printf("Setting GL attributes\n");
    if (setGLAttributes() < 0)
    {
        printf("Setting attributes failed. Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    printf("Creating Window\n");
    SDL_Window *window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, SCREEN_FLAGS);

    if (window == NULL)
    {
        printf("Window could not be created. Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    printf("Creating OpenGL context\n");
    SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    
    printf("%s\n", glGetString(GL_VERSION));

    if (SDL_GL_SetSwapInterval(VSYNC) != 0)
    {
        printf("Could not enable vsync. Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }

    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE);

    resizeViewport(window);

    return window;
}

GLuint setUpShaderProgram(App *app)
{
    printf("Loading shaders\n");
    GLuint shaders[2];
    size_t shaderCount = 2;

    char *vsSource = readResource("shaders/vertex.glsl", app);
    char *fsSource = readResource("shaders/fragment.glsl", app);

    if(vsSource == NULL || fsSource == NULL)
        return 0;

    if ((shaders[0] = loadShader(vsSource, GL_VERTEX_SHADER)) == 0)
        return 0;
    if ((shaders[1] = loadShader(fsSource, GL_FRAGMENT_SHADER)) == 0)
        return 0;

    free(vsSource);
    free(fsSource);

    printf("Creating shader program\n");
    GLuint shaderProgram = createProgram(shaders, shaderCount);

    for (int i = 0; i < shaderCount; i++)
        glDeleteShader(shaders[i]);

    return shaderProgram;
}

int loadVAOWithTextures(VertexAttributeObject **out, App *app, GLuint shaderProgram)
{
    printf("Creating VAO\n");

    float vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3};
    
    VertexAttribute vertexAttributes[3];
    vertexAttributes[0] = (VertexAttribute){0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0};
    vertexAttributes[1] = (VertexAttribute){1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))};
    vertexAttributes[2] = (VertexAttribute){2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))};

    *out = createVAOStruct(createVAO(vertices, sizeof(vertices), indices, sizeof(indices), vertexAttributes, 3), 6);

    Image *shipImage = readImageRsrc("img/Ship.png", app, true);
    if(shipImage == NULL)
        return 1;
    addTexToVAO(*out, createTexture(shipImage, GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE), "texture0", shaderProgram);
    freeImage(shipImage);

    Image *shieldImage = readImageRsrc("img/Shield.png", app, true);
    if(shieldImage == NULL)
        return 1;
    addTexToVAO(*out, createTexture(shieldImage, GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE), "texture1", shaderProgram);
    freeImage(shieldImage);

    Image *waterImage = readImageRsrc("img/water-normal.png", app, true);
    if(waterImage == NULL)
        return 1;
    addTexToVAO(*out, createTexture(waterImage, GL_RGBA, GL_LINEAR, GL_REPEAT), "texture2", shaderProgram);
    freeImage(waterImage);

    Image *causticsImage = readImageRsrc("img/rt-caustics-grayscale.png", app, true);
    if(causticsImage == NULL)
        return 1;
    addTexToVAO(*out, createTexture(causticsImage, GL_RGBA, GL_LINEAR, GL_REPEAT), "texture3", shaderProgram);
    freeImage(causticsImage);
    return 0;
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

void draw(App *app, GLuint shaderProgram, VertexAttributeObject **VAOs, size_t VAOCount)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    GLuint ticksPos = glGetUniformLocation(shaderProgram, "ticks");
    int ticks = SDL_GetTicks64();
    glUniform1i(ticksPos, ticks);

    for (int i = 0; i < VAOCount; i++)
    {
        for(int j = 0; j < VAOs[i]->textureCount; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, VAOs[i]->textures[j]);
        }
        glBindVertexArray(VAOs[i]->vao);
        glDrawElements(GL_TRIANGLES, VAOs[i]->count, GL_UNSIGNED_INT, 0);
    }
}

int main(int argc, char *argv[])
{

    // Set up window.

    SDL_Window *window;

    if ((window = setUpWindow()) == NULL)
        return 1;

    // Set up app.

    App *app;

    if ((app = setUpApp(window)) == NULL)
    {
        SDL_DestroyWindow(window);
        return 1;
    }

    // Set up shader program

    GLuint shaderProgram;

    if((shaderProgram = setUpShaderProgram(app)) == 0)
        return closeApp(app, 1);

    // VAOs.

    GLuint VAOCount = 1;
    VertexAttributeObject *VAOs[VAOCount];

    if(loadVAOWithTextures(&VAOs[0], app, shaderProgram))
        return closeApp(app, 1);

    printf("Done!\n");

    // Run gameloop.

    while (app->running)
    {
        // Poll events such as resizing window, hitting keys, or exiting the app.
        pollEvents(app);

        // Draw to screen
        draw(app, shaderProgram, VAOs, VAOCount);

        // Flip buffers.
        SDL_GL_SwapWindow(app->window);
    }

    // Close app.

    for(int i = 0; i < VAOCount; i++)
        free(VAOs[i]);

    return closeApp(app, 0);
}
