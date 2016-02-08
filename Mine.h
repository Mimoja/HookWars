#pragma once

#include "GameObject.h"
#include "LightSources.h"

class Mine : public GameObject {
    public:
        Mine();
        virtual void update();

    private:
        void explode();
        float respawntime;
        bool exists;
	PointLight* sight;
};

glm::vec3 spawn();
