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
    public:
        glm::vec3 vel;
        int owner;
        Chain(int player, glm::vec3 origin, glm::vec3 velocity, Chain* next, bool grapple);
        Chain(int player, glm::vec3 origin, glm::vec3 velocity, Hook*  hook, bool grapple);
        void kill();
        void pull();
        bool grappling;
		virtual void update();
		Chain* next;
		Chain* prev;
		Hook*  hook;
};
