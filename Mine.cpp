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

Mine::Mine() : GameObject(MINE_MODEL) {
    mModel.position = spawn();
    mModel.rotation.y = MINE_BASE_ROTATION;
    mModel.scaling = glm::vec3(MINE_SCALING, MINE_SCALING, MINE_SCALING);
    mModel.diffuseTexture = new Texture(MINE_TEXTURE);
    mModel.normalTexture = new Texture(MINE_TEXTURE_NORMAL);
    radius = MINE_RADIUS;
    allUpdateObjects.push_back(this);
    allRenderObjects.push_back(this);

    sight = new PointLight();
    sight->lightColor = glm::vec3(1.0f, 0.0f, 0.0f);
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
        for (auto o : allUpdateObjects) {
            if (o != (GameObject*)this && isColliding(*(GameObject*)this, *o)) {
                explode();
                if( std::find(allPlayers.begin(), allPlayers.end(), (Player*)o) != allPlayers.end() ) {
                    ((Player*) o)->hit();
                }
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
}

void Mine::explode() {
    mModel.position.y = -1000.0f;
    exists = false;
    respawntime = glfwGetTime() + MINE_RESPAWN_TIME;
}
