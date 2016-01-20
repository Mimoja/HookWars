#include "GameObject.h"
#include <vector>

#pragma once

class Hook : public GameObject {
    private:
        GameObject* next = NULL;
        int player;

    public:
        bool pulling;
        glm::vec3 pos, vel;
        Hook(int player, glm::vec3 origin, float dir);
        virtual void update();
        void pull();
		void kill();
		int collided;
};
