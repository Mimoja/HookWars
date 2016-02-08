#pragma once

#include "GameObject.h"
#include "LightSources.h"

class Mine : public GameObject {
    public:
        Mine(bool repair = false);
        virtual void update();
        bool exists;

    private:
        void explode();
        float respawndelay;
        float damage;
        float respawntime;
        bool isRepair;
	PointLight* sight;
};

glm::vec3 spawn();
