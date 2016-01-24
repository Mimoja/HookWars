#include "Chain.h"
#include "config.h"
#include <algorithm>

extern std::vector<Player*> allPlayers;
extern std::vector<GameObject*> allRenderObjects;

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
	allRenderObjects.push_back(this);
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

	allRenderObjects.erase(std::remove(allRenderObjects.begin(), allRenderObjects.end(), this), allRenderObjects.end());
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
		// pull
		Chain* p = prev;
		do {
			if (p != NULL) {
				follow = p->mModel.position;
				p = p->prev;
			} else {
				follow = allPlayers[owner]->mModel.position;
			}
			dif = follow - mModel.position;
		} while(glm::length(dif) == 0);

		if(glm::length(dif) > CHAIN_DISTANCE) {
			mModel.position += CHAIN_PULL * normalize(dif);
		} else {
			mModel.position += CHAIN_BASE_PULL * normalize(dif);
		}
		// update chain lasst because we are pulling
		if(next != NULL) {
			next->update();
		} else {
			hook->update();
		}
	} else {
		// update chain first because we are pushing
		if(next != NULL) {
			next->update();
		} else {
			hook->update();
		}
		// push
		if(next != NULL) {
			follow = next->mModel.position;
		} else {
			follow = hook->mModel.position;
		}
		dif = follow - mModel.position;
		if(glm::length(dif) != 0){
			if(glm::length(dif) > CHAIN_DISTANCE) {
				mModel.position += (glm::length(dif) - CHAIN_DISTANCE) * normalize(dif);
			} else {
				mModel.position += CHAIN_BASE_PUSH * normalize(dif);
			}
		}
	}
	glm::vec3 ne = (next != 0) ? next->mModel.position : hook->mModel.position;
	glm::vec3 pr = (prev != 0) ? prev->mModel.position : allPlayers[owner]->mModel.position;
	dif = ne-pr;
	mModel.rotation.y = glm::atan(dif.x, dif.z) + CHAIN_BASE_ROTATION + glm::pi<float>();

}
