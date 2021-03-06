#include "Hook.h"
#include "config.h"
#include "util.h"
#include "Mine.h"
#include <string>
#include <algorithm>

extern std::vector<GameObject*> allRenderObjects;

void Hook::pull() {
    pulling = true;
}

extern std::vector<Player*> allPlayers;
extern std::vector<Mine*> allMines;

extern GameObject* referenceHook;
extern GameObject* referenceGrapple;

Hook::Hook(int playerNumber, glm::vec3 origin, float dir, PointLight* p, bool grapple)
        : GameObject(*referenceHook) {
    if(grapple) {
        mModel = referenceGrapple->mModel;
    }
    grappling = grapple;
    owner = playerNumber;
    mModel.position = origin;
    vel = HOOK_SPEED * glm::normalize(glm::vec3(sin(dir), 0, cos(dir)));
    collided = 5;
    prev = NULL;
    pulling = false;
    pulled = NULL;
    mModel.rotation.y = dir + HOOK_BASE_ROTATION;
    mModel.scaling = glm::vec3(HOOK_SCALING, HOOK_SCALING, HOOK_SCALING);
    radius = HOOK_RADIUS;
    allRenderObjects.push_back(this);
    sight = p;
    sight->position = mModel.position;
    sight->position.y += 0.1f;
}

void Hook::update() {

    if (pulling) {
        if (!grappling) {
            // pull
            glm::vec3 follow;
            glm::vec3 dif;

            if (prev != NULL) {
                follow = prev->mModel.position;
            } else {
                follow = allPlayers[owner]->hookpoint;
            }
            dif = moveTowards(mModel.position, follow, CHAIN_BASE_PULL) - mModel.position;

            mModel.position += dif;

            mModel.rotation.y = glm::atan(dif.x, dif.z) + HOOK_BASE_ROTATION + glm::pi<float>();
            if(pulled != NULL){
                if( std::find(allMines.begin(), allMines.end(), pulled) == allMines.end() || ((Mine*)pulled)->exists) {
                    pulled->mModel.position = mModel.position;
                }
            }
        }
    } else {
        // push
        glm::vec3 normal = circleCollision(mModel.position, radius, 8.0f, false, false, allPlayers[owner]);

        if (collided == 0 && glm::length(normal) != 0.0f) {
            if(grappling) {
                // grapple
                allPlayers[owner]->pull();
            } else {
                // reflect
                vel = HOOK_SPEED * glm::normalize(glm::reflect(vel, glm::normalize(normal)));
                mModel.rotation.y = glm::atan(vel.x, vel.z) + HOOK_BASE_ROTATION;
                collided = 3;
            }
        } else {
            // just keep going
            mModel.position += vel;
            collided = std::max(0, collided - 1);
        }
    }
    // check for hits
    if (!grappling && pulled == NULL){
        // did we hit a player?
        for (Player* p : allPlayers) {
            if (p->playerNumber != owner && isColliding(*this, *p) && !p->isHit()) {
                pulled = p;
                p->hit();
                allPlayers[owner]->pull();
            }
        }
        // did we hit a mine?
        for (Mine* m : allMines) {
            if (isColliding(*this, *m)) {
                pulled = m;
                allPlayers[owner]->pull();
            }
        }
    }
    sight->position = mModel.position;
    sight->position.y += 0.1f;
}

void Hook::kill() {
    allRenderObjects.erase(std::remove(allRenderObjects.begin(), allRenderObjects.end(), this), allRenderObjects.end());
    delete this;
}
