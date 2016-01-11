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

std::vector<GameObject> allGameObjects;

GLFWwindow* window;
void mainLoop(void);

glm::mat4 Projection;
Camera cam;

int main(void){

    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		exit(1);
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
		exit(1);
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		exit(1);
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
        
double lastTime;
double nowTime;

void mainLoop(void){

    nowTime = glfwGetTime();

    // Clear the screen
	glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
    
    if(nowTime-lastTime>(1/60)){
        for(GameObject o : allGameObjects){
            o.update();
        }
        lastTime = glfwGetTime();
    }

    for(GameObject o : allGameObjects){
        o.render(0, Projection, cam);
    }
 
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

}
