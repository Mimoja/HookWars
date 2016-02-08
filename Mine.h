#include "GameObject.h"

#pragma once

class Mine : public GameObject {
    public:
        Mine(glm::vec3 pos);
        virtual void update();

    private:
        void explode();
        float respawntime;
        bool exists;
};

glm::vec3 spawn();
