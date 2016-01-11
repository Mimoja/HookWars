
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#include "util.h"

float del = 2;
double lastTime = 0.0f;
int nbFrames = 0;

void FPS_init(float delay) {
	lastTime= glfwGetTime();
	nbFrames = 0;
}
void FPS_count(){
	double currentTime = glfwGetTime();
	nbFrames++;
	if ( currentTime - lastTime >= del ){
		float t = (1000.0*del)/double(nbFrames);
		printf("%f ms/frame, %.1f frames / second\n",t,1000.0f/t);
		nbFrames = 0;
		lastTime = glfwGetTime(); // TODO can we add the delay and save time?
	}
}
