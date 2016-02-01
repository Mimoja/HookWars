#include "Chain.h"
#include "config.h"
#include "util.h"
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
		} while(glm::length(follow - mModel.position) == 0);
		
		this->moveTowards(follow, CHAIN_PULL, CHAIN_BASE_PULL);

		// update chain lasst because we are pulling
		this->updateChainLinks();
	} else {
		// update chain first because we are pushing
		this->updateChainLinks();

		// push
		if(next != NULL) {
			follow = next->mModel.position;
		} else {
			follow = hook->mModel.position;
		}

		this->moveTowards(follow, HOOK_SPEED, CHAIN_BASE_PUSH);
	}
	glm::vec3 ne = (next != 0) ? next->mModel.position : hook->mModel.position;
	glm::vec3 pr = (prev != 0) ? prev->mModel.position : allPlayers[owner]->mModel.position;
	glm::vec3 dif = ne-pr;
	mModel.rotation.y = glm::atan(dif.x, dif.z) + CHAIN_BASE_ROTATION + glm::pi<float>();
}

void Chain::updateChainLinks(){
	if(next != NULL) {
		next->update();
	} else {
		hook->update();
	}
}

void Chain::moveTowards(glm::vec3 target, float maxspeed, float minspeed) {
	float speed;
	glm::vec3 dif = target - mModel.position;
	glm::vec3 normal = circleCollision(mModel.position, HOOK_RADIUS, 4, false);

	if(glm::length(normal) > 0) {
		maxspeed *= 1.5f;
	}

	if(glm::length(dif) > CHAIN_DISTANCE) {
		speed = std::min(maxspeed, glm::length(dif) - CHAIN_DISTANCE);
	} else {
		speed = minspeed;
	}

	if (glm::length(normal) == 0 || glm::dot(normal, dif) > 0.0f) {
		mModel.position += speed * glm::normalize(dif);
	} else {
		mModel.position += speed * slideAlong(glm::normalize(dif), glm::normalize(normal));
	}
}
