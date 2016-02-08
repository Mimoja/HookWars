#include "Mine.h"
#include "config.h"
#include "util.h"

#include <algorithm>
#include <vector>
#include <stdlib.h>

extern std::vector<GameObject*> allUpdateObjects;
extern std::vector<GameObject*> allRenderObjects;
extern std::vector<Player*> allPlayers;
extern Lights allLightSources;

float randf(float min, float max) {
    return (float)((double)(rand()) / (double)(RAND_MAX) * ((double)max - (double)min) + (double)min);
}

glm::vec3 spawn() {
    glm::vec3 cand;
    do {
        cand = glm::vec3(randf(-10.0f, 10.0f), 2.0f, randf(-10.0f, 10.0f));
    } while (glm::length(circleCollision(cand, MINE_RADIUS, 4, true, true)) > 0.0f);

    return cand;
}

Mine::Mine(bool repair) : GameObject(MINE_MODEL) {
    radius = MINE_RADIUS;
    isRepair = repair;

    allUpdateObjects.push_back(this);
    allRenderObjects.push_back(this);

    sight = new PointLight();
    if(repair) {
        sight->lightColor = glm::vec3(0.3f, 1.0f, 0.3f);
        damage = -REPAIR_AMOUNT;
        mModel = Model(REPAIR_MODEL);
        mModel.scaling = glm::vec3(REPAIR_SCALING);
        mModel.diffuseTexture = new Texture(REPAIR_TEXTURE);
        mModel.normalTexture = new Texture(REPAIR_TEXTURE_NORMAL);
        mModel.rotation.y = REPAIR_BASE_ROTATION;
    } else {
        sight->lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
        damage = MINE_DAMAGE;
        mModel.rotation.y = MINE_BASE_ROTATION;
        mModel.scaling = glm::vec3(MINE_SCALING);
        mModel.diffuseTexture = new Texture(MINE_TEXTURE);
        mModel.normalTexture = new Texture(MINE_TEXTURE_NORMAL);
    }
    mModel.position = spawn();
    sight->intensity = 30.0f;
    sight->position = this->mModel.position;
    sight->position.y += 1.5f;
    sight->falloff.linear = 0.2f;
    sight->falloff.exponential = 0.5f;
    sight->specular.intensity = 0.0f;
    allLightSources.pointLights.push_back(sight);
}

void Mine::update() {
    if (exists) {
        for (auto p : allPlayers) {
            if (isColliding(*(GameObject*)this, *p)) {
                explode();
                if(!isRepair) {
                    p->hit();
                }
                p->health -= damage;
            }
        }
        sight->position = this->mModel.position;
        sight->position.y += 1.5f;
    } else {
        if (glfwGetTime() > respawntime) {
            mModel.position = spawn();
            exists = true;
        }
    }

    // rotate slowly
    mModel.rotation.y += 0.05;
}

void Mine::explode() {
    mModel.position.y = -1000.0f;
    exists = false;
    respawntime = glfwGetTime() + MINE_RESPAWN_TIME;
}
