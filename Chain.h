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
        bool pulling;
        Chain(PlayerColor player, glm::vec3 origin, glm::vec3 velocity, Chain* next);
        void pull();
        void kill();
};
