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

    glm::vec3 navEntry = getNavigationEntry(mModel.position + movementVector * PLAYER_MAXSPEED);
    if (navEntry.r != -1.0f && navEntry.r != 1.0f) {
        mModel.position += movementVector*PLAYER_MAXSPEED;
    }
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
			if (glm::length(mModel.position - chain->pos) > CHAIN_DISTANCE){
				// TODO Spawn additional chain link
			}
		} else {
			if (glm::length(mModel.position -  hook->pos) > CHAIN_DISTANCE){
				// TODO Spawn first chain link
			}
		}
	}

	// Move Light Source
    sight->position.x = mModel.position.x;
    sight->position.z = mModel.position.z;
}
