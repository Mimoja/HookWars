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
GameObject* referenceChain;
GameObject* referenceHook;

int WindowWidth = WINDOW_WIDTH;
int WindowHeight = WINDOW_HEIGHT;

GLuint shadowTexture;
GLuint frameBuffer = 0;

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
            JoystickPlayer* newPlayer = new JoystickPlayer(x);
            newPlayer->CONTROLER_NAME = joystickName;
            newPlayer->joystickAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + x,
                    &newPlayer->joystickAxisCount);
            newPlayer->calibrate();
            allPlayers.push_back(newPlayer);
            allUpdateObjects.push_back(newPlayer);
            allRenderObjects.push_back(newPlayer);
        }
    }
    if (allPlayers.size() != 0) {
        printf("%d Player found\n", (int) allPlayers.size());
    } else {
        printf("Falling back to keyboard players\n");
        for (int x = 0; x < 2; x++) {
            KeyboardPlayer* newPlayer = new KeyboardPlayer(x);
            allPlayers.push_back(newPlayer);
            allUpdateObjects.push_back(newPlayer);
            allRenderObjects.push_back(newPlayer);
        }
    }

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

    referenceHook = new GameObject(HOOK_MODEL);
    referenceChain = new GameObject(CHAIN_MODEL);
    map_ptr = new GameObject(MAP_MODEL);
    map_ptr->mModel.scaling = glm::vec3(MAP_SCALING);
    map_ptr->mModel.position = glm::vec3(0.0f);
    map_ptr->mModel.rotation = glm::vec3(0.0f);
    map_ptr->mModel.diffuseTexture = new Texture(MAP_DIFFUSE);
    map_ptr->mModel.normalTexture = new Texture(MAP_NORMAL);
    map_ptr->mModel.ssaoTexture = new Texture(MAP_AO);
    allRenderObjects.push_back(map_ptr);

    allLightSources.ambient.intensity = 0.6f;
    allLightSources.ambient.lightColor = glm::vec3(1.0f);

    PointLight* point1 = new PointLight();
    point1->lightColor = glm::vec3(1.0f, 1.0f, 0.8f);
    point1->intensity = 92.0f;
    point1->position = glm::vec3(0.0f, 10.5f, 2.0f);
    point1->falloff.linear = 0.0f;
    point1->falloff.exponential = 0.5f;
    point1->specular.intensity = 100.3f;
    point1->specular.power = 32;
    allLightSources.pointLights.push_back(point1);

    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTexture, 0);

    // No color output in the bound framebuffer, only depth.
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        exit(-1);
    }
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
    //Update
    if (nowTime - lastUpdateTime > (1 / 60)) {
        for (unsigned int i = 0; i < allUpdateObjects.size(); i++) {
            allUpdateObjects[i]->update();
        }
        cam.handleKeyboard(window);
        lastUpdateTime = glfwGetTime();
    }

    //Render
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, 1024, 1024); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glm::vec3 lightInvDir = glm::vec3(0.0f, 10.0f, 0.05f);
    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-20, 20, -20, 20, -20, 100);
    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 depthVP = depthProjectionMatrix * depthViewMatrix;

    for (unsigned int i = 0; i < allRenderObjects.size(); i++) {
        allRenderObjects[i]->renderShadow(shadowShaderID,depthVP);
    }

    //
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WindowWidth, WindowHeight);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
            );

    glm::mat4 depthBiasMVP = biasMatrix*depthVP;
    
    for (unsigned int i = 0; i < allRenderObjects.size(); i++) {
        allRenderObjects[i]->render(geometrieShaderID, Projection * cam.getView(), cam, allLightSources, depthBiasMVP);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}
