#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <stdio.h>

#include "GameObject.h"
#include "util.h"
#include "config.h"
#include "Camera.h"
#include "BoundingBox.h"
#include "Shader.h"
#include "Player.h"

std::vector<GameObject> allGameObjects;

GLFWwindow* window;
void mainLoop(void);

glm::mat4 Projection;
Camera cam;
std::vector<Player> allPlayers;

GLuint basicShaderID;
GLuint shadowShaderID;
ShadowMap map;

Lights allLightSources;

void window_size_callback(GLFWwindow* window, int width, int height) {
    glfwMakeContextCurrent(window);
    Projection = glm::perspective(45.0f, (float) width / (float) height, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
    map.create(width, height);
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


    // Joystick handle
    for (int x = 0; x < 16; x++) {
        const char* joystickName = glfwGetJoystickName(GLFW_JOYSTICK_1 + x);
        if (joystickName != 0) {
            printf("Found Joystick %s\n", joystickName);
            Player newPlayer(PLAYER_MODEL);
            newPlayer.CONTROLER_NAME = joystickName;
            newPlayer.mModel.scaling = glm::vec3(PLAYER_SCALING, PLAYER_SCALING, PLAYER_SCALING);
            allPlayers.push_back(newPlayer);
            allGameObjects.push_back(newPlayer);
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

    cam = Camera(glm::vec3(0.0f, 8.0f, 6.0f),
        glm::vec3(0.0f, -7.0f, -5.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    cam.setDomain(glm::vec3(-5.0f, 3.0f, 0.0f), glm::vec3(5.0f, 20.0f, 15.0f));

    // Compile Shaders
    printf("Compiling Shaders\n");

    basicShaderID = buildShader("shader.vs", "shader.fs");
    shadowShaderID = buildShader("shadows.vs", "shadows.fs");

    // Create ShadowMap
    map.create(WINDOW_WIDTH, WINDOW_HEIGHT);


#define loadCube
#ifndef loadCube
    GameObject map(MAP_MODEL);
    map.mModel.setScaling(MAP_SCALING, MAP_SCALING, MAP_SCALING);
    map.mModel.setPosition(0, 0, 0);
    map.mModel.setRotation(0, 0, 0);
#else
    GameObject map("assets/testcube.obj");
    map.mModel.scaling = glm::vec3(10.0f, 0.5f, 10.0f);
    map.mModel.position = glm::vec3(0, -1.5, 0);
    map.mModel.rotation = glm::vec3(0, 0, 0);
#endif
    allGameObjects.push_back(map);

    // Create lights
    allLightSources.ambient.intensity = 0.05f;
    allLightSources.ambient.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

    DirectionLight dir1;
    dir1.direction = glm::vec3(3.0f, 0.0f, -1.0f);
    dir1.intensity = 0.3f;
    dir1.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    dir1.specular.intensity = 1.0f;
    dir1.specular.power = 32;

    DirectionLight dir2;
    dir2.direction = glm::vec3(-3.0f, 3.0f, -1.0f);
    dir2.intensity = 0.05f;
    dir2.lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
    dir2.specular.intensity = 0.7f;
    dir2.specular.power = 32;

    allLightSources.directionalLights.push_back(dir1);
    allLightSources.directionalLights.push_back(dir2);

    PointLight point1;
    point1.lightColor = glm::vec3(0.1f, 0.3f, 0.8f);
    point1.intensity = 12.0f;
    point1.position = glm::vec3(0.0f, 4.5f, 3.0f);
    point1.falloff.linear = 0.0f;
    point1.falloff.exponential = 1.0f;
    point1.specular.intensity = 2.7f;
    point1.specular.power = 32;

    allLightSources.pointLights.push_back(point1);


    SpotLight spot1;
    spot1.lightColor = glm::vec3(0.3f, 0.8f, 0.1f);
    spot1.intensity = 10.0f;
    spot1.position = glm::vec3(6.0f, 4.5f, 13.0f);
    spot1.falloff.linear = 1.2f;
    spot1.falloff.exponential = 2.9f;
    spot1.specular.intensity = 0.7f;
    spot1.specular.power = 32;
    spot1.cutoff = 5.0f;
    spot1.direction = glm::vec3(0.0f, -1.0f, 7.0f);

    allLightSources.spotLights.push_back(spot1);

    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    FPS_init(2);
    long frameCount = 0;
    do {
        mainLoop();
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

long frameCount = 0;

void mainLoop(void) {

    nowTime = glfwGetTime();
    for (unsigned int j = 0; j < allPlayers.size(); j++) {
        allPlayers[j].joystickAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1,
            &allPlayers[j].joystickAxisCount);
        allPlayers[j].joystickButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1,
            &allPlayers[j].joystickButtonsCount);
    }


    if (nowTime - lastUpdateTime > (1 / 61)) {
        for (GameObject o : allGameObjects) {
            o.update();
        }
        for (Player p : allPlayers) {
            p.update();
        }
        lastUpdateTime = glfwGetTime();
    }
    cam.handleKeyboard(window);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (GameObject o : allGameObjects) {
        o.render(basicShaderID, Projection * cam.getView(), cam, allLightSources);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
    FPS_count();
    frameCount++;
}
