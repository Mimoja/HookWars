#include "GameObject.h"
#include "config.h"
#include <vector>

#pragma once

class Chain : public GameObject {
    private:
        GameObject* next;
        GameObject* prev;

    public:
        glm::vec3 pos, vel, rot;
        PlayerColor owner;
        Chain(PlayerColor player, glm::vec3 origin, glm::vec3 velocity);
        void kill();
};
