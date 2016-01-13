#include "Hook.h"
#include "config.h"

Hook::Hook(int player, glm::vec3 origin, glm::vec3 velocity) : GameObject(){
    linkpositions = {};
}

void Hook::pull(){
    pulling = true;
}

glm::vec3 Hook::origin(){
    // TODO: Implement this
    return glm::vec3(0);
}

void Hook::update(){
    // Is the chain moving outwards or inwards?
    if(pulling) {
        linkpositions.pop_back();
        glm::vec3 prev = origin();
        for (auto it = linkpositions.rbegin();it != linkpositions.rend(); it++) {
            glm::vec3 dist = *it - prev;
            *it += glm::normalize(dist)*LINK_DISTANCE;
        }
    } else {
        // TODO: Implement this
    }
}
