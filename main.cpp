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

std::vector<GameObject> allGameObjects;

GLFWwindow* window;
void mainLoop(void);

glm::mat4 Projection;
Camera cam;
int joysticks = 0;

GLuint basicShaderID;

int main(void){

    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // Multisampling
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Force ogl 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for reasons -_-
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT,WINDOW_NAME, NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW Window\n" );
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

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
    const char* joy1Name=glfwGetJoystickName(GLFW_JOYSTICK_1);
    const char* joy2Name=glfwGetJoystickName(GLFW_JOYSTICK_2);

    if(joy1Name!=0){
        printf("Found Joystick %s\n",joy1Name);
        joysticks++;
    }
    if(joy2Name!=0){
        printf("Found Joystick %s\n",joy1Name);
        joysticks++;
    }
    printf("%d Joysticks found\n",joysticks);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Projection = glm::perspective( 45.0f,
                                   (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,
                                   0.1f, 100.0f);

    cam = Camera( glm::vec3(0.0f,4.0f,4.0f),
                  glm::vec3(0.0f,-3.0f,-4.0f),
                  glm::vec3(0.0f,1.0f,0.0f));

    cam.setDomain(glm::vec3(-5.0f,0.0f,0.0f), glm::vec3(5.0f,10.0f,10.0f));

    // Compile Shaders
    printf("Compiling Shaders\n");

    basicShaderID = buildShader("shader.vs","shader.fs");

    GameObject cube("cube.obj");
    cube.mModel.setScaling(1.0f,1.0f,1.0f);
    cube.mModel.setPosition(0,0,0);
    cube.mModel.setRotation(0,0,0);

    allGameObjects.push_back(cube);


    FPS_init(2000);
    long frameCount = 0;
    do{
        mainLoop();
        frameCount++;
        FPS_count();
    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

double lastUpdateTime;
double nowTime;

const float* joyAxis1;
const float* joyAxis2;
const unsigned char* joyButtons1;
const unsigned char* joyButtons2;

int joyAxisCount1;
int joyAxisCount2;
int joyButtonsCount1;
int joyButtonsCount2;


void mainLoop(void){

    nowTime = glfwGetTime();
    if(joysticks>=1){
        joyAxis1 = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &joyAxisCount1);
        joyButtons1 = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &joyButtonsCount1);
        //for(int a = 0; a<joyAxisCount1; a++)
        //printf("%f from asix %d\n",joyAxis1[a],a);
    }

    if(joysticks==2){
        joyAxis2 = glfwGetJoystickAxes(GLFW_JOYSTICK_2, &joyAxisCount2);
        joyButtons2 = glfwGetJoystickButtons(GLFW_JOYSTICK_2, &joyButtonsCount2);
    }

    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );

    if(nowTime-lastUpdateTime>(1/60)){
        for(GameObject o : allGameObjects){
            o.update();
        }
        lastUpdateTime = glfwGetTime();
    }

    for(GameObject o : allGameObjects){
        o.render(basicShaderID, Projection*cam.getView(), cam);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

}
