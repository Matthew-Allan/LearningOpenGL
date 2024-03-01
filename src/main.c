#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "shader.h"
#include "app.h"
#include "buffers.h"
#include "texture.h"
#include "transforms.h"
#include "object.h"

#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 800
#define SCREEN_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

#define GLAD_MAJOR_VERSION 4
#define GLAD_MINOR_VERSION 1
#define GLAD_PROFILE_MASK SDL_GL_CONTEXT_PROFILE_CORE

#define POLYGON_MODE GL_FILL

#define CLEAR_COLOUR 0.2f, 0.3f, 0.3f, 1.0f

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

    printf("GL Version: %s\n", glGetString(GL_VERSION));

    if (SDL_GL_SetSwapInterval(VSYNC) != 0)
    {
        printf("Could not enable vsync. Error: %s\n", SDL_GetError());
        //SDL_DestroyWindow(window);
        //SDL_Quit();
        //return NULL;
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(CLEAR_COLOUR);

    glCullFace(GL_FRONT);

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

    if (vsSource == NULL || fsSource == NULL)
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

WorldObject *loadCubeWithTextures(App *app, GLuint shaderProgram, vec3 pos)
{
    printf("Creating Object\n");

    float vertices[] = {
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23};

    VertexAttribute vertexAttributes[3];
    vertexAttributes[0] = (VertexAttribute){0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0};
    vertexAttributes[1] = (VertexAttribute){1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))};
    vertexAttributes[2] = (VertexAttribute){2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))};

    WorldObject *cube = createWorldObject(createVAO(vertices, sizeof(vertices), indices + 0, sizeof(indices), vertexAttributes, 3), 36);

    Image *shipImage = readImageRsrc("img/Ship.png", app, true);
    Image *shieldImage = readImageRsrc("img/Shield.png", app, true);
    Image *waterImage = readImageRsrc("img/water-normal.png", app, true);
    Image *causticsImage = readImageRsrc("img/rt-caustics-grayscale.png", app, true);

    if (shipImage == NULL || shieldImage == NULL || waterImage == NULL || causticsImage == NULL)
        return NULL;

    addTexToWorldObject(cube, createTexture(shipImage, GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE), "texture0", shaderProgram);
    addTexToWorldObject(cube, createTexture(shieldImage, GL_RGBA, GL_NEAREST, GL_CLAMP_TO_EDGE), "texture1", shaderProgram);
    addTexToWorldObject(cube, createTexture(waterImage, GL_RGB, GL_LINEAR, GL_REPEAT), "texture2", shaderProgram);
    addTexToWorldObject(cube, createTexture(causticsImage, GL_RGB, GL_LINEAR, GL_REPEAT), "texture3", shaderProgram);

    freeImages(app->images, &app->images, 4);

    cube->pos = pos;

    return cube;
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
            updateDimentions(app);
            break;
        default:
            break;
        }
    }
}

void draw(App *app, GLuint shaderProgram, WorldObject **objects, size_t objectCount)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    GLuint ticksPos = glGetUniformLocation(shaderProgram, "ticks");
    int ticks = SDL_GetTicks64();
    glUniform1i(ticksPos, ticks);

    for (int i = 0; i < objectCount; i++)
    {
        for (int j = 0; j < objects[i]->textureCount; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, objects[i]->textures[j]);
        }

        mat4 model = identMat4;
        mat4 projection = identMat4;
        mat4 view = translationMat4(0, 0, -3.0);

        genRotationMatrix(&model, rad(-55.0f), vec3(1, 0, 0));
        rotate((vec4 *)&model, (vec4 *)&model, 4, ticks / 4000.0f, vec3(0.5f, 1.0f, 0.0f));
        translate((vec4 *)&model, (vec4 *)&model, 4, objects[i]->pos);
        perspective(&projection, rad(45), (float)app->w / (float)app->h, 0.1f, 100.0f);
        //orthographic(&projection, -ASPECT_RATIO(app) * 6, ASPECT_RATIO(app) * 6, -6, 6, 0.1, 100);

        GLuint modelPos = glGetUniformLocation(shaderProgram, "model");
        GLuint viewPos = glGetUniformLocation(shaderProgram, "view");
        GLuint projectionPos = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelPos, 1, GL_FALSE, vecPos(model));
        glUniformMatrix4fv(viewPos, 1, GL_FALSE, vecPos(view));
        glUniformMatrix4fv(projectionPos, 1, GL_FALSE, vecPos(projection));

        glBindVertexArray(objects[i]->vao);
        glDrawElements(GL_TRIANGLES, objects[i]->vertexCount, GL_UNSIGNED_INT, 0);
    }
}

int main(int argc, char *argv[])
{

    // Set up window.

    SDL_Window *window = setUpWindow();
    if (window == NULL)
        return 1;

    // Set up app.

    App *app = setUpApp(window);
    if (app == NULL)
    {
        SDL_DestroyWindow(window);
        return 1;
    }

    // Set up shader program

    GLuint shaderProgram = setUpShaderProgram(app);
    if (shaderProgram == 0)
        return closeApp(app, 1);

    // Objects.

    GLuint objectCount = 10;
    WorldObject *objects[objectCount];

    if ((objects[0] = loadCubeWithTextures(app, shaderProgram, vec3(0, 0, 0))) == NULL)
        return closeApp(app, 1);
    if ((objects[1] = loadCubeWithTextures(app, shaderProgram, vec3(2, 5, -15))) == NULL)
        return closeApp(app, 1);
    if ((objects[2] = loadCubeWithTextures(app, shaderProgram, vec3(-1.5, -2.2, -2.5))) == NULL)
        return closeApp(app, 1);
    if ((objects[3] = loadCubeWithTextures(app, shaderProgram, vec3(-3.8f, -2.0f, -12.3f))) == NULL)
        return closeApp(app, 1);
    if ((objects[4] = loadCubeWithTextures(app, shaderProgram, vec3(2.4f, -0.4f, -3.5f))) == NULL)
        return closeApp(app, 1);
    if ((objects[5] = loadCubeWithTextures(app, shaderProgram, vec3(-1.7f, 3.0f, -7.5f))) == NULL)
        return closeApp(app, 1);
    if ((objects[6] = loadCubeWithTextures(app, shaderProgram, vec3(1.3f, -2.0f, -2.5f))) == NULL)
        return closeApp(app, 1);
    if ((objects[7] = loadCubeWithTextures(app, shaderProgram, vec3(1.5f, 2.0f, -2.5f))) == NULL)
        return closeApp(app, 1);
    if ((objects[8] = loadCubeWithTextures(app, shaderProgram, vec3(1.5f, 0.2f, -1.5f))) == NULL)
        return closeApp(app, 1);
    if ((objects[9] = loadCubeWithTextures(app, shaderProgram, vec3(-1.3f, 1.0f, -1.5f))) == NULL)
        return closeApp(app, 1);

    printf("Done!\n");

    // Run gameloop.

    while (app->running)
    {
        // Poll events such as resizing window, hitting keys, or exiting the app.
        pollEvents(app);

        // Draw to screen
        draw(app, shaderProgram, objects, objectCount);

        // Flip buffers.
        SDL_GL_SwapWindow(app->window);
    }

    // Close app.

    for (int i = 0; i < objectCount; i++)
        free(objects[i]);

    return closeApp(app, 0);
}
