#include "Chain.h"
#include "config.h"

extern std::vector<GameObject*> allGameObjects;

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity)
		: GameObject(CHAIN_MODEL){
    owner = player;
    mModel.position = origin;
    vel = velocity;
	prev = NULL;
	next = NULL;
	hook = NULL;
	mModel.rotation.y = glm::atan(vel.x, vel.z) + CHAIN_BASE_ROTATION;
	mModel.scaling = glm::vec3(CHAIN_SCALING, CHAIN_SCALING, CHAIN_SCALING);
	allGameObjects.push_back(this);
}

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next)
		: Chain(player, origin, velocity){
	this->next = next;
}

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity, Hook*  hook)
		: Chain(player, origin, velocity){
	this->hook = hook;
}

void Chain::kill(){
    
}

void Chain::update(){
	glm::vec3 dif;
	if(next != NULL) {
		dif = (next->mModel.position - mModel.position);
	} else {
		dif = (hook->mModel.position - mModel.position);
	}
	mModel.position += 0.15f * dif;
	mModel.rotation.y = 0.3f * mModel.rotation.y + 0.7f * glm::atan(dif.x, dif.z) + 0.7* CHAIN_BASE_ROTATION;
	mModel.position = mModel.position;
	
}
