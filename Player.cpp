#include "Player.h"
#include "config.h"
#include "stdio.h"
#include "util.h"

extern std::vector<GameObject*> allUpdateObjects;
extern std::vector<GameObject*> allRenderObjects;
extern Lights allLightSources;
extern std::vector<glm::vec3> spawnPoints;

extern GameObject* referenceHook;

enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
    RTL,
    RTR,
    FIRE,
    GRAPPLE,
};
int keyboardControls[2][8] = {
    {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_H, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L},
    {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T}
};

void KeyboardPlayer::update() {
    // update chain first if we are pushing
    if (hook != NULL && !pulling) {
        if (chain != NULL) {
            chain->update();
        } else {
            hook->update();
        }
    }

    extern GLFWwindow* window;
    movementVector = glm::vec3(0.0f);
    if (glfwGetKey(window, keyboardControls[playerNumber][UP]) == GLFW_PRESS)movementVector.z = -1.0f;
    else if (glfwGetKey(window, keyboardControls[playerNumber][DOWN]) == GLFW_PRESS)movementVector.z = 1.0f;
    if (glfwGetKey(window, keyboardControls[playerNumber][LEFT]) == GLFW_PRESS)movementVector.x = -1.0f;
    else if (glfwGetKey(window, keyboardControls[playerNumber][RIGHT]) == GLFW_PRESS)movementVector.x = 1.0f;

    fire = glfwGetKey(window, keyboardControls[playerNumber][FIRE]) == GLFW_PRESS;
    grapple = glfwGetKey(window, keyboardControls[playerNumber][GRAPPLE]) == GLFW_PRESS;

    if (glfwGetKey(window, keyboardControls[playerNumber][RTL]) == GLFW_PRESS)rotation += 0.1f;
    if (glfwGetKey(window, keyboardControls[playerNumber][RTR]) == GLFW_PRESS)rotation -= 0.1f;

    rotationVector.x = sin(rotation);
    rotationVector.y = cos(rotation);

    Player::update();
}

void JoystickPlayer::update() {
    // update chain first if we are pushing
    if (hook != NULL && !pulling) {
        if (chain != NULL) {
            chain->update();
        } else {
            hook->update();
        }
    }

    joystickAxis = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + playerNumber, &joystickAxisCount);
    joystickButtons = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + playerNumber, &joystickButtonsCount);

    if (fabs(joystickAxis[XBOX_ONE_GAMEPAD::MOVE_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        movementVector.x = joystickAxis[XBOX_ONE_GAMEPAD::MOVE_LEFT_RIGHT] - joystickCalibration[0].x;
    } else movementVector.x = 0.0f;
    if (fabs(joystickAxis[XBOX_ONE_GAMEPAD::MOVE_UP_DOWN]) > GAMEPAD_CUTOFF) {
        movementVector.z = -(joystickAxis[XBOX_ONE_GAMEPAD::MOVE_UP_DOWN] - joystickCalibration[0].z);
    } else movementVector.z = 0.0f;

    rotationVector.x = -(joystickAxis[XBOX_ONE_GAMEPAD::TURN_LEFT_RIGHT] - joystickCalibration[1].x);
    rotationVector.y = joystickAxis[XBOX_ONE_GAMEPAD::TURN_UP_DOWN] - joystickCalibration[1].y;
    // workaround so abs(rotationVector) is never 0
    if (rotationVector.x == 0) rotationVector.x = 0.00001f;

    fire = (joystickAxis[XBOX_ONE_GAMEPAD::FIRE] < 0);
    grapple = (joystickAxis[XBOX_ONE_GAMEPAD::GRAPPLE] > 0);

    Player::update();
}

void JoystickPlayer::calibrate() {
    joystickCalibration[0].x = joystickAxis[XBOX_ONE_GAMEPAD::MOVE_LEFT_RIGHT];
    joystickCalibration[0].z = joystickAxis[XBOX_ONE_GAMEPAD::MOVE_UP_DOWN];
    joystickCalibration[1].x = joystickAxis[XBOX_ONE_GAMEPAD::TURN_LEFT_RIGHT];
    joystickCalibration[1].y = joystickAxis[XBOX_ONE_GAMEPAD::TURN_UP_DOWN];
}

Player::Player(int number) : GameObject(PLAYER_MODEL) {
    hook = NULL;
    chain = NULL;
    pulling = false;
    health = 1.0f;
    radius = PLAYER_RADIUS;
    hookpoint = mModel.position;
    lastHookTime = glfwGetTime();
    lastGrappleTime = glfwGetTime();
    lastHitTime = glfwGetTime() - BLINK_TIME;
    new Rotor(this, -0.15f, 1.5f);
    mModel.scaling = glm::vec3(PLAYER_SCALING);
    mModel.rotation = glm::vec3(0.0f);
    mModel.diffuseTexture = new Texture(PLAYER_TEXTURE);
    mModel.normalTexture = new Texture(PLAYER_TEXTURE_NORMAL);
    mModel.ssaoTexture = new Texture(PLAYER_TEXTURE_SSAO);
    playerNumber = number;

    hookSight = new PointLight();
    hookSight->lightColor = glm::vec3(1.0f, 0.3f, 0.1f);
    hookSight->intensity = 22.0f;
    hookSight->position = glm::vec3(0.0f, -10.5f, 2.0f);
    hookSight->falloff.linear = 0.2f;
    hookSight->falloff.exponential = 0.2f;
    hookSight->specular.intensity = 8.3f;
    hookSight->specular.power = 32;

    sight = new PointLight();
    sight->lightColor = glm::normalize(glm::vec3(fmod(number+2*0.1f,1.0f), fmod(0.6f*number,1.0f), fmod(1.0f-number/2,1.0f)));
    sight->intensity = 10.0f;
    sight->position = glm::vec3(0.0f, 4.0f, 3.0f);
    sight->falloff.linear = 0.2f;
    sight->falloff.exponential = 0.4f;
    sight->specular.intensity = 0.0f;

    canonSight = new PointLight();
    canonSight->lightColor = glm::vec3(1.0f, 0.4f, 0.7f);
    canonSight->intensity = 15.0f;
    canonSight->position = glm::vec3(0.0f, 1.5f, 3.0f);
    canonSight->falloff.linear = 0.5f;
    canonSight->falloff.exponential = 0.5f;
    canonSight->specular.intensity = 0.0f;

    allLightSources.pointLights.push_back(canonSight);
    allLightSources.pointLights.push_back(sight);
    allLightSources.pointLights.push_back(hookSight);

    healthBar = new HealthBar(this);
    allRenderObjects.push_back(healthBar);
    allUpdateObjects.push_back(healthBar);
}

void Player::update() {

    glm::vec3 normal = circleCollision(mModel.position, PLAYER_RADIUS, 8, true, true, this);

    if (glm::length(normal) == 0 || glm::dot(normal, movementVector) > 0.0f) {
        mModel.position += PLAYER_MAXSPEED * movementVector;
    } else {
        mModel.position += PLAYER_MAXSPEED * slideAlong(movementVector, glm::normalize(normal));
    }

    hookpoint = mModel.position + PLAYER_SCALING * glm::vec3(rotationVector.x, 0.2f, rotationVector.y);

    glm::vec3 navEntry = getNavigationEntry(mModel.position + movementVector * PLAYER_MAXSPEED);

    // in Trap?
    if (navEntry.b > 0.9f) {
        if(!isHit()) {
            hit();
        }
    }

    mModel.rotation.y = glm::atan(rotationVector.x, rotationVector.y) + PLAYER_BASE_ROTATION;

    double now = glfwGetTime();

    // blink?
    if(now - lastHitTime < BLINK_TIME && fmod(now, BLINK_INTERVAL) < BLINK_INTERVAL/3.0f) {
        mModel.position.y = -200.0f;
    } else {
        mModel.position.y = 2.0f;
    }

    // Fire or Pull
    if (fire) {
        if (hook == NULL && now - lastHookTime > HOOK_COOLDOWN) {
            // Fire new hook
            lastHookTime = now;
            hookSight->lightColor = glm::vec3(1.0f, 0.3f, 0.1f);
            hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION, hookSight);
        } else if (hook != NULL && !(hook->grappling) && now - lastHookTime > HOOK_RETRACT_TIME) {
            pull();
        }
    }

    // Grapple
    if (grapple) {
        if (hook == NULL && now - lastGrappleTime > GRAPPLE_COOLDOWN) {
            // Fire new grapple
            lastGrappleTime = now;
            lastHookTime = now;
            hookSight->lightColor = glm::vec3(0.3f, 1.0f, 0.1f);
            hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION, hookSight, true);
        }
    }

    // Pull automatically if Hook too long
    if (hook != NULL && now - lastHookTime > HOOK_LIFETIME) {
        pull();
    }

    // Extend/Retract Hook if needed
    if (hook != NULL) {
        if (pulling) {
            if (chain != NULL) {
                if (hook->grappling) {
                    mModel.position.x = chain->mModel.position.x;
                    mModel.position.z = chain->mModel.position.z;
                    auto next = chain->next;
                    chain->kill();
                    chain = next;
                } else {
                    if (glm::length(hookpoint - chain->mModel.position) < 1.5f * CHAIN_DISTANCE) {
                        auto next = chain->next;
                        chain->kill();
                        chain = next;
                    }
                }
            } else {
                if (hook->grappling) {
                    mModel.position.x = hook->mModel.position.x;
                    mModel.position.z = hook->mModel.position.z;                        
                    hook->kill();
                    hook = NULL;
                    pulling = false;
                } else {
                    if (glm::length(hookpoint - hook->mModel.position) < 1.5f * CHAIN_DISTANCE) {
                        if(hook->grappling) { mModel.position = hook->mModel.position; };
                        hook->kill();
                        hook = NULL;
                        pulling = false;
                    }
                }
            }
        } else {
            if (chain != NULL) {
                if (glm::length(hookpoint - chain->mModel.position) > CHAIN_DISTANCE) {
                    chain = new Chain(playerNumber, hookpoint, hook->mModel.position - mModel.position, chain, hook->grappling);
                }
            } else {
                if (glm::length(hookpoint - hook->mModel.position) > CHAIN_DISTANCE) {
                    chain = new Chain(playerNumber, hookpoint, hook->mModel.position - mModel.position, hook, hook->grappling);
                }
            }
        }
    }

    // update chain last if we are pulling
    if (hook != NULL && pulling) {
        if (chain != NULL) {
            chain->update();
        } else {
            hook->update();
        }
    }

    if (hook == NULL) {
        hookSight->lightColor = glm::vec3(0.0f);
    }

    // Move Light Source
    sight->position.x = mModel.position.x;
    sight->position.z = mModel.position.z;

    canonSight->position.x = mModel.position.x + glm::normalize(glm::vec3(rotationVector.x, 0.0f, rotationVector.y)).x * 2.0f;
    canonSight->position.z = mModel.position.z + glm::normalize(glm::vec3(rotationVector.x, 0.0f, rotationVector.y)).z * 2.0f;

}

void Player::pull() {
    pulling = true;
    if (chain == NULL) {
        hook->pull();
    } else {
        chain->pull();
    }
}

bool Player::isHit() {
    return glfwGetTime() - lastHitTime < BLINK_TIME;
}

void Player::hit() {
    health -= 0.2f;
    lastHitTime = glfwGetTime();
    if (health <= 0.01f) {
        die();
    }
}

void Player::die() {
    health = 1.0f;
    mModel.position = spawnPoints[playerNumber];
}

Rotor::Rotor(Player* owner, float rotation, float height) : GameObject(ROTOR_MODEL) {
    player = owner;
    mModel.position.y = height;
    mModel.scaling = owner->mModel.scaling;
    rot = rotation;
    allUpdateObjects.push_back(this);
    allRenderObjects.push_back(this);
}

void Rotor::update() {
    mModel.position.x = player->mModel.position.x;
    mModel.position.z = player->mModel.position.z;
    mModel.rotation.y += rot;
}
