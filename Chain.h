#pragma once

#include "Player.h"
#include "config.h"
#include <vector>
#include "Hook.h"

class Hook;

class Chain : public GameObject {
	private:
        Chain(int player, glm::vec3 origin, glm::vec3 velocity);
		bool pulling;
		void updateChainLinks();
		void moveTowards(glm::vec3 target, float maxspeed, float minspeed);
    public:
        glm::vec3 vel;
        int owner;
        Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next);
        Chain(int player, glm::vec3 origin, glm::vec3 velocity, Hook*  hook);
        void kill();
        void pull();
		virtual void update();
		Chain* next;
		Chain* prev;
		Hook*  hook;
};
