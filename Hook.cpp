#include "Hook.h"
#include "Chain.h"
#include "config.h"
#include "util.h"
#include <string>
#include <algorithm>

void Hook::pull(){
    pulling = true;
}

extern std::vector<GameObject*> allGameObjects;

Hook::Hook(int playerNumber, glm::vec3 pos, float dir) : GameObject(HOOK_MODEL){
	this->player = playerNumber;
	this->pos = pos;
	printf("%f, %f, %f\nNEU\n", pos.x, pos.y, pos.z);
	this->vel = HOOK_SPEED * glm::normalize(glm::vec3(sin(dir), 0, cos(dir)));
	this->collided = 0;
	mModel.rotation.y = dir + HOOK_BASE_ROTATION;
	mModel.scaling = glm::vec3(HOOK_SCALING, HOOK_SCALING, HOOK_SCALING);
	allGameObjects.push_back(this);
}

void Hook::update(){
	mModel.position = pos;

	// check for collisions in circle
	glm::vec3 normal = glm::vec3(0, 0, 0);
	for (float a = 0; a < 2 * glm::pi<float>(); a += glm::quarter_pi<float>()/4){
		glm::vec3 offset = HOOK_RADIUS * glm::vec3(sin(a), 0, cos(a));
		glm::vec3 navEntry = getNavigationEntry(mModel.position + offset);
		if (navEntry.r == -1.0f || navEntry.r == 1.0f) {
        	normal -= offset;
    	}
	}
	
	glm::vec3 navEntry = getNavigationEntry(mModel.position);
	if (collided == 0 && (navEntry.r == -1.0f || navEntry.r == 1.0f)) {
		// reflect
		printf("poop\n");
		normal = glm::normalize(normal);
		vel = HOOK_SPEED * glm::normalize(glm::reflect(vel, normal));
		printf("%f, %f\n", vel.x, vel.z);
		mModel.rotation.y = glm::atan(vel.x, vel.z) + HOOK_BASE_ROTATION;
		collided = 10;
	} else {
		// just keep going
		pos += vel;
		collided = std::max(0, collided - 1);
	}
}

void Hook::kill(){
	allGameObjects.erase(std::remove(allGameObjects.begin(), allGameObjects.end(), this), allGameObjects.end());
	delete this;
}
