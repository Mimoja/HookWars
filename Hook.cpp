#include "Hook.h"

Hook::Hook(int player, glm::vec3 origin, glm::vec3 velocity) : GameObject(){
    linkpositions = {};
}

void Hook::pull(){
    pulling = true;
}
