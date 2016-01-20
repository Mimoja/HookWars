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
	glm::vec3 normal = circleCollision(pos, HOOK_RADIUS, 8.0f);

	if (collided == 0 && glm::length(normal) != 0.0f) {
		// reflect
		printf("poop\n");
		vel = HOOK_SPEED * glm::normalize(glm::reflect(vel, glm::normalize(normal)));
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
