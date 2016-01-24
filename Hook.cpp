#include "Hook.h"
#include "config.h"
#include "util.h"
#include <string>
#include <algorithm>

void Hook::pull(){
    pulling = true;
}

extern std::vector<Player*> allPlayers;

Hook::Hook(int playerNumber, glm::vec3 origin, float dir) : GameObject(HOOK_MODEL){
	owner = playerNumber;
	mModel.position = origin;
	vel = HOOK_SPEED * glm::normalize(glm::vec3(sin(dir), 0, cos(dir)));
	collided = 0;
	prev = NULL;
	pulling = false;
	mModel.rotation.y = dir + HOOK_BASE_ROTATION;
	mModel.scaling = glm::vec3(HOOK_SCALING, HOOK_SCALING, HOOK_SCALING);
    radius = HOOK_RADIUS;
}

void Hook::update(){

	if(pulling) {
		// pull
		glm::vec3 follow;
		glm::vec3 dif;

		if(prev != NULL){
			follow = prev->mModel.position;
		} else {
			follow = allPlayers[owner]->mModel.position;
		}
		dif = follow - mModel.position;

		if(glm::length(dif) > CHAIN_DISTANCE) {
			mModel.position += CHAIN_PULL * normalize(dif);
		} else {
			mModel.position += CHAIN_BASE_PULL * normalize(dif);
		}

		mModel.rotation.y = glm::atan(dif.x, dif.z) + HOOK_BASE_ROTATION + glm::pi<float>();
	} else {
		// push
		glm::vec3 normal = circleCollision(mModel.position, radius, 8.0f, false);

		if (collided == 0 && glm::length(normal) != 0.0f) {
			// reflect
			vel = HOOK_SPEED * glm::normalize(glm::reflect(vel, glm::normalize(normal)));
			mModel.rotation.y = glm::atan(vel.x, vel.z) + HOOK_BASE_ROTATION;
			collided = 10;
		} else {
			// just keep going
			mModel.position += vel;
			collided = std::max(0, collided - 1);
		}
        for(Player* p : allPlayers){
            if(p->playerNumber!= owner && isColliding(*this,*p)){
                printf("Hit Player %d",p->playerNumber);
            }
        }
        
	}
}

void Hook::kill(){
	delete this;
}
