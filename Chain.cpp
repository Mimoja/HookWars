#include "Chain.h"
#include "config.h"

void Chain::pull(){
    pulling = true;
}

Chain::Chain(PlayerColor player, glm::vec3 origin, glm::vec3 velocity, Chain* nextlink){
    owner = player;
    pos = origin;
    vel = velocity;
    rot = glm::vec3(0);
    next = nextlink;
}

void Chain::kill(){
    
}
