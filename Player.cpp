#include "Player.h"
#include "config.h"
#include "stdio.h"
#include "util.h"

void Player::calibrate() {
    joystickCalibration[0].x = joystickAxis[MOVE_LEFT_RIGHT];
    joystickCalibration[0].z = joystickAxis[MOVE_UP_DOWN];
    joystickCalibration[1].x = joystickAxis[TURN_LEFT_RIGHT];
    joystickCalibration[1].y = joystickAxis[TURN_UP_DOWN];
}

Player::Player(const char* file) : GameObject(file) {
	hook = NULL;
	chain = NULL;
}

void Player::update() {

    if (fabs(joystickAxis[MOVE_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        movementVector.x = joystickAxis[MOVE_LEFT_RIGHT] - joystickCalibration[0].x;
    } else movementVector.x = 0.0f;
    if (fabs(joystickAxis[MOVE_UP_DOWN]) > GAMEPAD_CUTOFF) {
        movementVector.z = joystickAxis[MOVE_UP_DOWN] - joystickCalibration[0].z;
    } else movementVector.z = 0.0f;

    rotationVector.x = joystickAxis[TURN_LEFT_RIGHT] - joystickCalibration[1].x;
    rotationVector.y = joystickAxis[TURN_UP_DOWN] - joystickCalibration[1].y;



    // TODO check collision

	glm::vec3 normal = circleCollision(mModel.position, PLAYER_RADIUS, 8);

    if (glm::length(normal) == 0 || glm::dot(normal, movementVector) > 0.0f) {
        mModel.position += PLAYER_MAXSPEED * movementVector;
    } else {
		mModel.position += PLAYER_MAXSPEED * slideAlong(movementVector, glm::normalize(normal));
	}

    glm::vec3 navEntry = getNavigationEntry(mModel.position + movementVector * PLAYER_MAXSPEED);
    if (navEntry.g == 1.0f) {
        printf("In River!\n");
    }
    if (navEntry.b > 0.9f) {
        printf("In Trap\n");
    }
    mModel.rotation.y = glm::atan(rotationVector.x, rotationVector.y) + PLAYER_BASE_ROTATION;

    // Fire
    double now = glfwGetTime();
    if (joystickAxis[FIRE] > 0
        && now - lastHookTime > HOOK_COOLDOWN
		/* && hook == NULL */) {
        lastHookTime = now;
        printf("Hook fired\n");
		hook->kill();
        hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION);
    }

	// Extend Hook if needed
	if (hook != NULL) {
		if (chain != NULL) {
			if (glm::length(mModel.position - chain->mModel.position) > CHAIN_DISTANCE){
				chain = new Chain(playerNumber, mModel.position, hook->mModel.position - mModel.position, chain);
			}
		} else {
			if (glm::length(mModel.position -  hook->mModel.position) > CHAIN_DISTANCE){
				chain = new Chain(playerNumber, mModel.position, hook->mModel.position - mModel.position, hook);
			}
		}
	}

	// Move Light Source
    sight->position.x = mModel.position.x;
    sight->position.z = mModel.position.z;
}
