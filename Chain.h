#pragma once

#include "GameObject.h"
#include "config.h"
#include <vector>
#include "Hook.h"

class Hook;

class Chain : public GameObject {

    public:
        glm::vec3 pos, vel, rot;
        int owner;
        Chain(int player, glm::vec3 origin, glm::vec3 velocity);
        Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next);
        void kill();
		virtual void update();
		Chain* next;
		Chain* prev;
};
