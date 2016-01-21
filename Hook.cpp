#include "Hook.h"
#include "config.h"
#include "util.h"
#include <string>
#include <algorithm>

void Hook::pull(){
    pulling = true;
}

extern std::vector<GameObject*> allGameObjects;
extern std::vector<Player*> allPlayers;

Hook::Hook(int playerNumber, glm::vec3 origin, float dir) : GameObject(HOOK_MODEL){
	owner = playerNumber;
	mModel.position = origin;
	printf("%f, %f, %f\nNEU\n", mModel.position.x, mModel.position.y, mModel.position.z);
	vel = HOOK_SPEED * glm::normalize(glm::vec3(sin(dir), 0, cos(dir)));
	collided = 0;
	prev = NULL;
	pulling = false;
	mModel.rotation.y = dir + HOOK_BASE_ROTATION;
	mModel.scaling = glm::vec3(HOOK_SCALING, HOOK_SCALING, HOOK_SCALING);
	allGameObjects.push_back(this);
}

void Hook::update(){

	if(pulling) {
		// update chain first because we are pulling
		if(prev != NULL) {
			prev->update();
		}
		// pull
		glm::vec3 follow;
		glm::vec3 dif;

		if(prev != NULL){
			follow = prev->mModel.position;
		} else {
			follow = allPlayers[owner]->mModel.position;
		}
		dif = follow - mModel.position;
		mModel.position += std::min(CHAIN_DISTANCE, (glm::length(dif) - CHAIN_DISTANCE)) * normalize(dif);

		mModel.rotation.y = glm::atan(-dif.x, -dif.z) + HOOK_BASE_ROTATION;
	} else {
		// push
		glm::vec3 normal = circleCollision(mModel.position, HOOK_RADIUS, 8.0f);

		if (collided == 0 && glm::length(normal) != 0.0f) {
			// reflect
			vel = HOOK_SPEED * glm::normalize(glm::reflect(vel, glm::normalize(normal)));
			printf("%f, %f\n", vel.x, vel.z);
			mModel.rotation.y = glm::atan(vel.x, vel.z) + HOOK_BASE_ROTATION;
			collided = 10;
		} else {
			// just keep going
			mModel.position += vel;
			collided = std::max(0, collided - 1);
		}
		// update chain last because we are pushing
		if(prev != NULL) {
			prev->update();
		}
	}
}

void Hook::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights){
	GameObject::render(shaderID, MVP, camera, lights);
	for(Chain* p = prev; p != NULL; p = p->prev) {
		p->render(shaderID, MVP, camera, lights);
	}
}

void Hook::kill(){
	printf("HOOK KILLED!\n");
	allGameObjects.erase(std::remove(allGameObjects.begin(), allGameObjects.end(), this), allGameObjects.end());
	delete this;
}
