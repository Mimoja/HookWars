#include "Player.h"
#include "config.h"
#include "stdio.h"
#include "Hook.h"

void Player::calibrate() {
    joystickCalibration[0].x = joystickAxis[MOVE_LEFT_RIGHT];
    joystickCalibration[0].z = joystickAxis[MOVE_UP_DOWN];
    joystickCalibration[1].x = joystickAxis[TURN_LEFT_RIGHT];
    joystickCalibration[1].y = joystickAxis[TURN_UP_DOWN];
}

void Player::update() {

    if (fabs(joystickAxis[MOVE_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        movementVector.x = joystickAxis[MOVE_LEFT_RIGHT] - joystickCalibration[0].x;
    } else movementVector.x = 0.0f;
    if (fabs(joystickAxis[MOVE_UP_DOWN]) > GAMEPAD_CUTOFF) {
        movementVector.z = joystickAxis[MOVE_UP_DOWN] - joystickCalibration[0].z;
    } else movementVector.z = 0.0f;
    if (fabs(joystickAxis[TURN_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        rotationVector.x = joystickAxis[TURN_LEFT_RIGHT] - joystickCalibration[1].x;
    } else rotationVector.x = 0.0f;
    if (fabs(joystickAxis[TURN_UP_DOWN]) > GAMEPAD_CUTOFF) {
        rotationVector.y = joystickAxis[TURN_UP_DOWN] - joystickCalibration[1].y;
    } else rotationVector.y = 0.0f;

    if(rotationVector.x == 0.0f) {
        mModel.rotation.y = rotationVector.y < 0.0f ? 3*glm::half_pi<float>() : glm::half_pi<float>();
	} else {
        mModel.rotation.y = glm::atan(rotationVector.y / rotationVector.x);
    }
    // map 2 werte zwischen -1 und 1 für x und y auf einen rotationswert
    mModel.position += movementVector*PLAYER_MAXSPEED;

    // Fire
    double now  = glfwGetTime();
    if (joystickAxis[FIRE] > 0 
        && now - lastHookTime > HOOK_COOLDOWN) {
        lastHookTime = now;
        printf("Hook fired\n");
        //TODO Spawn Hook
    }

}
