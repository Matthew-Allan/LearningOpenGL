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
#include "camera.h"
#include "world.h"
#include "input.h"

#define DEFAULT_SCREEN_WIDTH 1600
#define DEFAULT_SCREEN_HEIGHT 800
#define SCREEN_FLAGS SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE

#define GLAD_MAJOR_VERSION 4
#define GLAD_MINOR_VERSION 1
#define GLAD_PROFILE_MASK SDL_GL_CONTEXT_PROFILE_CORE

#define POLYGON_MODE GL_FILL

#define CLEAR_COLOUR 0.2f, 0.3f, 0.3f, 1.0f

#define CAP_AT(value, min, max) (((value) > (max)) ? (max) : (((value) < (min)) ? (min) : (value)))

#define VSYNC true

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

WorldObject *loadCubeWithTextures(App *app, World *world, GLuint shaderProgram, vec3 pos)
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

    WorldObject *cube = createWorldObject(createVAO(vertices, sizeof(vertices), indices + 0, sizeof(indices), vertexAttributes, 3), 36, pos, ++world->prevId);

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

    app->images = freeImages(app->images, 4);

    return cube;
}

int loadObjectsIntoWorld(App *app, World *world, GLuint shaderProgram)
{
    GLuint objectCount = 10;
    WorldObject *objects[objectCount];

    if ((objects[0] = loadCubeWithTextures(app, world, shaderProgram, vec3(0, 0, 0))) == NULL)
        return 1;
    if ((objects[1] = loadCubeWithTextures(app, world, shaderProgram, vec3(2, 5, -15))) == NULL)
        return 1;
    if ((objects[2] = loadCubeWithTextures(app, world, shaderProgram, vec3(-1.5, -2.2, -2.5))) == NULL)
        return 1;
    if ((objects[3] = loadCubeWithTextures(app, world, shaderProgram, vec3(-3.8f, -2.0f, -12.3f))) == NULL)
        return 1;
    if ((objects[4] = loadCubeWithTextures(app, world, shaderProgram, vec3(2.4f, -0.4f, -3.5f))) == NULL)
        return 1;
    if ((objects[5] = loadCubeWithTextures(app, world, shaderProgram, vec3(-1.7f, 3.0f, -7.5f))) == NULL)
        return 1;
    if ((objects[6] = loadCubeWithTextures(app, world, shaderProgram, vec3(1.3f, -2.0f, -2.5f))) == NULL)
        return 1;
    if ((objects[7] = loadCubeWithTextures(app, world, shaderProgram, vec3(1.5f, 2.0f, -2.5f))) == NULL)
        return 1;
    if ((objects[8] = loadCubeWithTextures(app, world, shaderProgram, vec3(1.5f, 0.2f, -1.5f))) == NULL)
        return 1;
    if ((objects[9] = loadCubeWithTextures(app, world, shaderProgram, vec3(-1.3f, 1.0f, -1.5f))) == NULL)
        return 1;

    for(int i = 0; i < objectCount; i++)
        addWorldObject(world, objects[i]);

    return 0;
}

void pollEvents(App *app, World *world)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        // If the switch statement isn't the most truly ugly bit of syntax, I dont know what is.
        switch (e.type)
        {
        case SDL_QUIT:
            app->running = false;
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                resizeViewport(app->window);
            updateDimentions(app);
            setAspect(world->camera, ASPECT(app));
            break;
        case SDL_MOUSEWHEEL:
            app->input->scrollDelta += e.wheel.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            app->input->trackingMouse = !app->input->trackingMouse;
            if (SDL_SetRelativeMouseMode(app->input->trackingMouse) != 0)
            {
                printf("Could not enable relative mouse mode. Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(app->window);
                SDL_Quit();
                app->running = false;
            }
            break;
        default:
            break;
        }
    }
}

void draw(App *app, World *world, GLuint shaderProgram)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    GLuint ticksPos = glGetUniformLocation(shaderProgram, "ticks");
    Uint64 ticks = SDL_GetTicks64();
    glUniform1i(ticksPos, ticks);

    GLuint projectionPos = glGetUniformLocation(shaderProgram, "projection");
    GLuint viewPos = glGetUniformLocation(shaderProgram, "view");

    glUniformMatrix4fv(projectionPos, 1, GL_FALSE, vecPos(world->camera->projection));
    glUniformMatrix4fv(viewPos, 1, GL_FALSE, vecPos(world->camera->view));

    for (WorldObject *object = world->objects; object != NULL; object = object->next)
    {
        for (int j = 0; j < object->textureCount; j++)
        {
            glActiveTexture(GL_TEXTURE0 + j);
            glBindTexture(GL_TEXTURE_2D, object->textures[j]);
        }

        mat4 model = identMat4;

        rotate(vecArr4(model), vecArr4(model), 4, ticks / 4000.0f, vec3(0.5f, 1.0f, 0.0f));
        translate(vecArr4(model), vecArr4(model), 4, object->pos);

        GLuint modelPos = glGetUniformLocation(shaderProgram, "model");

        glUniformMatrix4fv(modelPos, 1, GL_FALSE, vecPos(model));

        glBindVertexArray(object->vao);
        glDrawElements(GL_TRIANGLES, object->vertexCount, GL_UNSIGNED_INT, 0);
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

    // Set up inputs.

    addAxis(app->input, createAxis("x", 2, (SDL_Scancode[5]){SDL_SCANCODE_RIGHT, SDL_SCANCODE_D}, 2, (SDL_Scancode[5]){SDL_SCANCODE_LEFT, SDL_SCANCODE_A}));
    addAxis(app->input, createAxis("y", 1, (SDL_Scancode[5]){SDL_SCANCODE_SPACE}, 2, (SDL_Scancode[5]){SDL_SCANCODE_LSHIFT,SDL_SCANCODE_RSHIFT}));
    addAxis(app->input, createAxis("z", 2, (SDL_Scancode[5]){SDL_SCANCODE_UP, SDL_SCANCODE_W}, 2, (SDL_Scancode[5]){SDL_SCANCODE_DOWN, SDL_SCANCODE_S}));

    // Set up world.

    World *world = createWorld();

    // Set up camera.

    Camera *camera = createCamera(vec3(0, 0, 3), vec3(0, 0, -1), ASPECT(app), PERSPECTIVE);
    setCamera(world, camera);

    // Set up shader program.

    GLuint shaderProgram = setUpShaderProgram(app);
    if (shaderProgram == 0)
    {
        freeWorld(world);
        closeApp(app);
        return 1;
    }

    // Objects.

    if (loadObjectsIntoWorld(app, world, shaderProgram))
    {
        freeWorld(world);
        closeApp(app);
        return 1;
    }

    // Run gameloop.

    float pitch = 0;
    float yaw = rad(-90);
    int fov = 45;

    while (app->running)
    {
        // Poll events such as resizing window, hitting keys, or exiting the app.
        pollEvents(app, world);

        float movementSpeed = 10.0f;
        float relativeSpeed = movementSpeed * (app->deltaTime / 1000.0f);

        Input *input = app->input;

        vec3 movementAxes = get3DAxisValue(input, "x", "y", "z");

        // Normalise movement axes and multiply by relative speed.
        norm3(movementAxes, &movementAxes);
        scalarMult3(&movementAxes, &movementAxes, 1, relativeSpeed);

        vec3 front;
        vec3 right;
        vec3 up;

        // Get right.
        cross(camera->dir, camera->worldUp, &right);
        norm3(right, &right);

        // Get front.
        cross(camera->worldUp, right, &front);
        norm3(front, &front);

        // Cancel out movement when movementAxes has values of 0.
        scalarMult3(&right, &right, 1, movementAxes.x);
        scalarMult3(&front, &front, 1, movementAxes.z);
        scalarMult3(&camera->worldUp, &up, 1, movementAxes.y);

        // Add vectors together to get the delta.
        vec3 movementVector;
        add3(&right, &front, &movementVector, 1);
        add3(&movementVector, &up, &movementVector, 1);

        // Add the delta to the camera pos
        vec3 newCamPos;
        add3(&movementVector, &camera->pos, &newCamPos, 1);

        // Set the camera pos to the new pos.
        setCamPos(camera, newCamPos);

        // Track mouse and scroll wheel and set FOV and direction as such.
        if(input->trackingMouse)
        {
            pitch -= input->mouseYDelta / 1000.0f;
            yaw += input->mouseXDelta / 1000.0f;
            printf("%f %f\n", pitch, yaw);
            fov -= input->scrollDelta;

            fov = CAP_AT(fov, 1, 110);
            pitch = CAP_AT(pitch, rad(-89), rad(89));

            setCamFOV(camera, rad(fov));

            setCamDir(world->camera, vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch)));
        }

        // Draw to screen
        draw(app, world, shaderProgram);

        // Flip buffers.
        SDL_GL_SwapWindow(app->window);

        // Update deltatime;
        tickFrame(app);
    }

    // Close app.

    freeWorld(world);
    closeApp(app);

    return 0;
}
