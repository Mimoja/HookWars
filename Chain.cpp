#include "Chain.h"
#include "config.h"
#include <algorithm>

extern std::vector<Player*> allPlayers;

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity)
		: GameObject(CHAIN_MODEL){
    owner = player;
    mModel.position = origin;
    vel = velocity;
	prev = NULL;
	next = NULL;
	hook = NULL;
	pulling = false;
	mModel.rotation.y = glm::atan(vel.x, vel.z) + CHAIN_BASE_ROTATION;
	mModel.scaling = glm::vec3(CHAIN_SCALING, CHAIN_SCALING, CHAIN_SCALING);
}

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next)
		: Chain(player, origin, velocity){
	this->next = next;
	next->prev = this;
}

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity, Hook*  hook)
		: Chain(player, origin, velocity){
	this->hook = hook;
	hook->prev = this;
}

void Chain::kill(){
	// invalidate pointers
	if(next != NULL) {
		next->prev = prev;
	} else {
		hook->prev = prev;
	}
	if(prev != NULL) {
		prev->next = next;
	}

	delete this;
}

void Chain::pull(){
    pulling = true;
	if(next != NULL) {
		next->pull();
	} else {
		hook->pull();
	}
}

void Chain::update(){
	glm::vec3 follow;
	glm::vec3 dif;

	if(pulling) {
		// update chain first because we are pulling
		if(prev != NULL) {
			prev->update();
		}
		// pull
		if(prev != NULL){
			follow = prev->mModel.position;
		} else {
			follow = allPlayers[owner]->mModel.position;
		}
		dif = follow - mModel.position;

		if(glm::length(dif) != 0) {
			mModel.position += CHAIN_BASE_PULL * normalize(dif);
		}
	} else {
		// push
		if(next != NULL) {
			follow = next->mModel.position;
		} else {
			follow = hook->mModel.position;
		}
		dif = follow - mModel.position;
		if(glm::length(dif) != 0){
			if(glm::length(dif) > CHAIN_DISTANCE) {
				mModel.position += std::min(CHAIN_DISTANCE, (glm::length(dif) - CHAIN_DISTANCE)) * normalize(dif);
			} else {
				mModel.position += CHAIN_BASE_PUSH * normalize(dif);
			}
		}
		// update chain last because we are pushing
		if(prev != NULL) {
			prev->update();
		}
	}

	mModel.rotation.y = glm::atan(dif.x, dif.z) + CHAIN_BASE_ROTATION + glm::pi<float>();


}
