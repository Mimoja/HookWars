#pragma once

#include "GameObject.h"
#include "LightSources.h"

class Mine : public GameObject {
    public:
        Mine();
        virtual void update();
        bool exists;

    private:
        void explode();
        float respawntime;
	PointLight* sight;
};

glm::vec3 spawn();
