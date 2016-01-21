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
	pulling = false;
}

void Player::update() {
	// update chain first if we are pushing
	if(hook != NULL && !pulling){
		if(chain != NULL) {
			chain->update();
		} else {
			hook->update();
		}
	}

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

    // Fire or Pull
    double now = glfwGetTime();
    if (joystickAxis[FIRE] > 0){
		if (hook == NULL && now - lastHookTime > HOOK_COOLDOWN) {
			// Fire new hook
		    lastHookTime = now;
		    hook = new Hook(playerNumber, mModel.position, mModel.rotation.y - PLAYER_BASE_ROTATION);
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
				if (glm::length(mModel.position - chain->mModel.position) < 1.2f*CHAIN_DISTANCE){
					auto next = chain->next;
					chain->kill();
					chain = next;
				}
			} else {
				if (glm::length(mModel.position -  hook->mModel.position) < 1.2f*CHAIN_DISTANCE){
					hook->kill();
					hook = NULL;
					pulling = false;
				}
			}
		} else {
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
	}

	// update chain last if we are pulling
	if(hook != NULL && pulling){
		if(chain != NULL) {
			chain->update();
		} else {
			hook->update();
		}
	}

	// Move Light Source
    sight->position.x = mModel.position.x;
    sight->position.z = mModel.position.z;
}

void Player::pull(){
	pulling = true;
	if (chain == NULL) {
		hook->pull();
	} else {
		chain->pull();
	}
}

void Player::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights){
	GameObject::render(shaderID, MVP, camera, lights);
	if(hook!= NULL) {	
		hook->GameObject::render(shaderID, MVP, camera, lights);
		for(Chain* p = chain; p != NULL; p = p->next) {
			p->render(shaderID, MVP, camera, lights);
		}
	}
}
