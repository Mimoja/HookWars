#include "Mine.h"
#include "config.h"
#include "util.h"

#include <algorithm>
#include <vector>
#include <stdlib.h>

extern std::vector<GameObject*> allUpdateObjects;
extern std::vector<GameObject*> allRenderObjects;
extern std::vector<Player*> allPlayers;

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

Mine::Mine(glm::vec3 pos) : GameObject(MINE_MODEL) {
    mModel.position = pos;
    mModel.rotation.y = MINE_BASE_ROTATION;
    mModel.scaling = glm::vec3(MINE_SCALING, MINE_SCALING, MINE_SCALING);
    mModel.diffuseTexture = new Texture(MINE_TEXTURE);
    mModel.normalTexture = new Texture(MINE_TEXTURE_NORMAL);
    radius = MINE_RADIUS;
    allUpdateObjects.push_back(this);
    allRenderObjects.push_back(this);
}

void Mine::update() {
    if (exists) {
        for (auto o : allUpdateObjects) {
            if (o != (GameObject*)this && isColliding(*(GameObject*)this, *o)) {
                explode();
            }
        }
    } else {
        if (glfwGetTime() > respawntime) {
            mModel.position = spawn();
            exists = true;
        }
    }
}

void Mine::explode() {
    radius = MINE_RANGE;
    for (auto o : allUpdateObjects) {
        if (std::find(allPlayers.begin(), allPlayers.end(), (Player*)o) != allPlayers.end()
                && isColliding(*this, *o)) {
            ((Player*)o)->hit();
        }
    }
    mModel.position.y = -1000.0f;
    exists = false;
    respawntime = glfwGetTime() + MINE_RESPAWN_TIME;
}
