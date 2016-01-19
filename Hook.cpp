#include "Hook.h"
#include "config.h"
#include "util.h"
#include <string>
#include <algorithm>

void Hook::pull(){
    pulling = true;
}

extern std::vector<GameObject*> allGameObjects;

Hook::Hook(int playerNumber, glm::vec3 pos, glm::vec3 vel) : GameObject("assets/derp.obj"){
	this->pos = pos;
	printf("%f, %f, %f\nNEU\n", pos.x, pos.y, pos.z);
	this->vel = HOOK_SPEED * glm::vec3(vel.x, 0, vel.y);
	allGameObjects.push_back(this);
}

void Hook::update(){
	pos += vel;
	printf("%f, %f, %f\n", this->pos.x, this->pos.y, this->pos.z);
	mModel.position = pos;
	mModel.rotation = vel;
}

void Hook::kill(){
	allGameObjects.erase(std::remove(allGameObjects.begin(), allGameObjects.end(), this), allGameObjects.end());
	delete this;
}
