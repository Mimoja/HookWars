#pragma once

#include "GameObject.h"
#include "Chain.h"
#include <vector>

class Chain;

class Hook : public GameObject {
    private:
        GameObject* next = NULL;
        int owner;

    public:
        bool pulling;
        glm::vec3 vel;
        Hook(int player, glm::vec3 origin, float dir);
        virtual void update();
        void pull();
		void kill();
		int collided;
		Chain* prev;
};
