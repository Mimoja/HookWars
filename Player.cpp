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

    rotationVector.x = joystickAxis[TURN_LEFT_RIGHT] - joystickCalibration[1].x;
    rotationVector.y = joystickAxis[TURN_UP_DOWN] - joystickCalibration[1].y;


    // map 2 werte zwischen -1 und 1 für x und y auf einen rotationswert
    mModel.position += movementVector*PLAYER_MAXSPEED;
    mModel.rotation.y = glm::atan( rotationVector.x,rotationVector.y )+PLAYER_BASE_ROTATION;

    // Fire
    double now  = glfwGetTime();
    if (joystickAxis[FIRE] > 0 
        && now - lastHookTime > HOOK_COOLDOWN) {
        lastHookTime = now;
        printf("Hook fired\n");
        //TODO Spawn Hook
    }

}
