#include "Player.h"
#include "config.h"
#include "stdio.h"
#include "util.h"

extern std::vector<GameObject*> allUpdateObjects;
extern std::vector<GameObject*> allRenderObjects;
extern Lights allLightSources;

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
int keyboardControls[2][7] = {
    {GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M},
    {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B}
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
        movementVector.z = joystickAxis[XBOX_ONE_GAMEPAD::MOVE_UP_DOWN] - joystickCalibration[0].z;
    } else movementVector.z = 0.0f;

    rotationVector.x = joystickAxis[XBOX_ONE_GAMEPAD::TURN_LEFT_RIGHT] - joystickCalibration[1].x;
    rotationVector.y = joystickAxis[XBOX_ONE_GAMEPAD::TURN_UP_DOWN] - joystickCalibration[1].y;

    fire = (joystickAxis[XBOX_ONE_GAMEPAD::FIRE] > 0);
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
    new Rotor(this, -0.15f, 2.0f);
    mModel.scaling = glm::vec3(PLAYER_SCALING);
    mModel.position = glm::vec3(-5.0f, 2.0f, 0.0f);
    mModel.rotation = glm::vec3(0.1f);
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
    sight->lightColor = glm::vec3(0.6f, 0.6f, 1.0f);
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

    hookpoint = mModel.position + 1.3f * glm::vec3(rotationVector.x, 0.2f, rotationVector.y);

    glm::vec3 navEntry = getNavigationEntry(mModel.position + movementVector * PLAYER_MAXSPEED);
    if (navEntry.g == 1.0f) {
        printf("In River!\n");
    }
    if (navEntry.b > 0.9f) {
        printf("In Trap\n");
    }
    mModel.rotation.y = glm::atan(rotationVector.x, rotationVector.y) + PLAYER_BASE_ROTATION;

    // Fire or Pull or Grapple
    double now = glfwGetTime();
    if (fire) {
        if (hook == NULL && now - lastHookTime > HOOK_COOLDOWN) {
            // Fire new hook
            lastHookTime = now;
            hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION, hookSight);
        } else if (hook != NULL && now - lastHookTime > HOOK_RETRACT_TIME) {
            pull();
        }
    }

    if (grapple) {
        if (hook == NULL && now - lastHookTime > HOOK_COOLDOWN) {
            // Fire new grapple
            lastHookTime = now;
            hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION, hookSight, true);
        } else if (hook != NULL && now - lastHookTime > HOOK_RETRACT_TIME) {
            pull();
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
                if (glm::length(hookpoint - chain->mModel.position) < 1.5f * CHAIN_DISTANCE) {
                    auto next = chain->next;
                    chain->kill();
                    chain = next;
                }
            } else {
                if (glm::length(hookpoint - hook->mModel.position) < 1.5f * CHAIN_DISTANCE) {
                    hook->kill();
                    hook = NULL;
                    pulling = false;
                }
            }
        } else {
            if (chain != NULL) {
                if (glm::length(hookpoint - chain->mModel.position) > CHAIN_DISTANCE) {
                    chain = new Chain(playerNumber, hookpoint, hook->mModel.position - mModel.position, chain);
                }
            } else {
                if (glm::length(hookpoint - hook->mModel.position) > CHAIN_DISTANCE) {
                    chain = new Chain(playerNumber, hookpoint, hook->mModel.position - mModel.position, hook);
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
    } else {
        hookSight->lightColor = glm::vec3(1.0f, 0.3f, 0.1f);
    }

    // Move Light Source
    sight->position.x = mModel.position.x;
    sight->position.z = mModel.position.z;

    canonSight->position = mModel.position + glm::normalize(glm::vec3(rotationVector.x, 0.0f, rotationVector.y))*2.0f;
    canonSight->position.y += 0.5f;

}

void Player::pull() {
    pulling = true;
    if (chain == NULL) {
        hook->pull();
    } else {
        chain->pull();
    }
}

Rotor::Rotor(Player* owner, float rotation, float height) : GameObject(ROTOR_MODEL) {
    player = owner;
    mModel.position.y = height;
    rot = rotation;
    allUpdateObjects.push_back(this);
    allRenderObjects.push_back(this);
}

void Rotor::update() {
    mModel.position.x = player->mModel.position.x;
    mModel.position.z = player->mModel.position.z;
    mModel.rotation.y += rot;
}
