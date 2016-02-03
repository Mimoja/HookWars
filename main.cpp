#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <stdio.h>

#include "lodepng.h"
#include "GameObject.h"
#include "util.h"
#include "config.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "Shader.h"
#include "Player.h"
#include "HealthBar.h"

std::vector<GameObject*> allUpdateObjects;
std::vector<GameObject*> allRenderObjects;
std::vector<Player*> allPlayers;

GLFWwindow* window;
void mainLoop(long frameCount);

glm::mat4 Projection;
Camera cam;

GLuint geometrieShaderID;
GLuint shadowShaderID;
GLuint healthBarShaderID;

Lights allLightSources;

std::vector<unsigned char> navigationMap;
unsigned navigationMapHeight;
unsigned navigationMapWidth;

GameObject* map_ptr;

int WindowWidth = WINDOW_WIDTH;
int WindowHeight = WINDOW_HEIGHT;

void window_size_callback(GLFWwindow* window, int width, int height) {
    glfwMakeContextCurrent(window);
    Projection = glm::perspective(45.0f, (float) width / (float) height, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
    WindowHeight = height;
    WindowWidth = width;
}

int main(void) {

    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // Multisampling
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Force ogl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for reasons -_-
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW Window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    // Read Navigation Map
    unsigned int res = lodepng::decode(navigationMap, navigationMapWidth, navigationMapHeight, NAVIGATION_MAP);

    if (res != 0) {
        printf("error %d : %s\n", res, lodepng_error_text(res));
        exit(-1);
    }

    // Joystick handle
    for (int x = 0; x < 16; x++) {
        const char* joystickName = glfwGetJoystickName(GLFW_JOYSTICK_1 + x);
        if (joystickName != 0) {
            printf("Found Joystick %s\n", joystickName);
            Player* newPlayer = new Player(PLAYER_MODEL);
            newPlayer->CONTROLER_NAME = joystickName;
            newPlayer->playerNumber = x;
    		newPlayer->mModel.position = glm::vec3(-5.0f, 2.0f, 0.0f);
            newPlayer->joystickAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + x,
                    &newPlayer->joystickAxisCount);
            newPlayer->calibrate();
            newPlayer->color = glm::vec3(0.9f, 0.0f, 0.1f);
            newPlayer->useColor = false;
            allPlayers.push_back(newPlayer);
            allUpdateObjects.push_back(newPlayer);
            allRenderObjects.push_back(newPlayer);

            PointLight* point1 = new PointLight();
            point1->lightColor = glm::vec3(0.7f, 0.7f, 1.0f);
            point1->intensity = 12.0f;
            point1->position = glm::vec3(0.0f, 4.5f, 3.0f);
            point1->falloff.linear = 0.0f;
            point1->falloff.exponential = 1.0f;
            point1->specular.intensity = 0.0f;
            point1->specular.power = 32;
            newPlayer->sight = point1;
            allLightSources.pointLights.push_back(point1);

            HealthBar* h = new HealthBar(newPlayer);
            allRenderObjects.push_back(h);
            allUpdateObjects.push_back(h);
        }
    }
    printf("%d Player found\n", (int) allPlayers.size());

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Projection = glm::perspective(45.0f,
            (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT,
            0.1f, 100.0f);

    cam = Camera(glm::vec3(-2.0f, 20.0f, 15.0f),
            glm::vec3(0.0f, -7.0f, -5.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));

    cam.setDomain(glm::vec3(-7.0f, 3.0f, 0.0f), glm::vec3(5.0f, 30.0f, 20.0f));

    // Compile Shaders
    printf("Compiling Shaders\n");

    geometrieShaderID = buildShader(GEOMETRIE_VERTEX, GEOMETRIE_FRAGMENT);
    shadowShaderID = buildShader(SHADOW_VERTEX, SHADOW_FRAGMENT);
    healthBarShaderID = buildShader(HEALTHBAR_VERTEX, HEALTHBAR_FRAGMENT);

    map_ptr = new GameObject(MAP_MODEL);
    map_ptr->mModel.scaling = glm::vec3(MAP_SCALING, MAP_SCALING, MAP_SCALING);
    map_ptr->mModel.position = glm::vec3(0, 0, 0);
    map_ptr->mModel.rotation = glm::vec3(0, 0, 0);
    map_ptr->mModel.diffuseTexture = new Texture();
    map_ptr->mModel.diffuseTexture->loadPNG(MAP_DIFFUSE);
    map_ptr->mModel.normalTexture = new Texture();
    map_ptr->mModel.normalTexture->loadPNG(MAP_NORMAL);
    allRenderObjects.push_back(map_ptr);

    allLightSources.ambient.intensity = 0.2f;
    allLightSources.ambient.lightColor = glm::vec3(1.0f);

    PointLight* point1 = new PointLight();
    point1->lightColor = glm::vec3(0.99f, 0.1f, 0.0f);
    point1->intensity = 22.0f;
    point1->position = glm::vec3(0.0f, 4.5f, 3.0f);
    point1->falloff.linear = 0.0f;
    point1->falloff.exponential = 1.0f;
    point1->specular.intensity = 0.0f;
    point1->specular.power = 32;
    allLightSources.pointLights.push_back(point1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    printf("Entering Main Loop\n");
    FPS_init(2);
    long frameCount = 0;
    do {
        mainLoop(frameCount);
        frameCount++;
        FPS_count();
    }// Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

double lastUpdateTime;
double nowTime;

void mainLoop(long frameCount) {

    nowTime = glfwGetTime();
    for (unsigned int j = 0; j < allPlayers.size(); j++) {
        allPlayers[j]->joystickAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + j,
                &allPlayers[j]->joystickAxisCount);
        allPlayers[j]->joystickButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + j,
                &allPlayers[j]->joystickButtonsCount);
    }

    if (nowTime - lastUpdateTime > (1 / 60)) {
        for (unsigned int i = 0; i < allUpdateObjects.size(); i++) {
            allUpdateObjects[i]->update();
        }
        cam.handleKeyboard(window);
        lastUpdateTime = glfwGetTime();
    }

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1000, 1000);
    for (unsigned int i = 0; i < allLightSources.spotLights.size(); i++) {
        SpotLight* s = allLightSources.spotLights[i];
        Camera lightCam(s->position, s->direction, glm::vec3(0.0f, 1.0f, 0.0f));
        for (unsigned int i = 0; i < allRenderObjects.size(); i++) {
            allRenderObjects[i]->renderShadow(shadowShaderID, Projection * lightCam.getView());
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WindowWidth, WindowHeight);
    for (unsigned int i = 0; i < allRenderObjects.size(); i++) {
        allRenderObjects[i]->render(geometrieShaderID, Projection * cam.getView(), cam, allLightSources);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}
