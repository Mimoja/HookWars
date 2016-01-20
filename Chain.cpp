#include "Chain.h"
#include "config.h"

extern std::vector<GameObject*> allGameObjects;

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity)
		: GameObject(CHAIN_MODEL){
    owner = player;
    pos = origin;
    vel = velocity;
    rot = glm::vec3(0);
	prev = NULL;
	next = NULL;
	mModel.rotation.y = glm::atan(vel.x, vel.z) + CHAIN_BASE_ROTATION;
	mModel.scaling = glm::vec3(CHAIN_SCALING, CHAIN_SCALING, CHAIN_SCALING);
	allGameObjects.push_back(this);
}

Chain::Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next)
		: Chain(player, origin, velocity){
	next = next;
}

void Chain::kill(){
    
}

void Chain::update(){
	mModel.position = pos;
}
