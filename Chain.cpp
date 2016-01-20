#include "Chain.h"
#include "config.h"

Chain::Chain(PlayerColor player, glm::vec3 origin, glm::vec3 velocity)
		: GameObject("assets/chain.obj") {
    owner = player;
    pos = origin;
    vel = velocity;
    rot = glm::vec3(0);
}

void Chain::kill(){
    
}
