#include "Player.h"
#include "config.h"
#include "stdio.h"

void Player::calibrate() {
    joystickCalibration[0].x = joystickAxis[MOVE_LEFT_RIGHT];
    joystickCalibration[0].z = joystickAxis[MOVE_UP_DOWN];
    joystickCalibration[1].y = joystickAxis[TURN_LEFT_RIGHT];
    joystickCalibration[1].z = joystickAxis[TURN_UP_DOWN];
}

void Player::update() {

    if (fabs(joystickAxis[MOVE_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        movementVector.x = joystickAxis[MOVE_LEFT_RIGHT] - joystickCalibration[0].x;
    } else movementVector.x = 0.0f;
    if (fabs(joystickAxis[MOVE_UP_DOWN]) > GAMEPAD_CUTOFF) {
        movementVector.z = joystickAxis[MOVE_UP_DOWN] - joystickCalibration[0].z;
    } else movementVector.z = 0.0f;
    if (fabs(joystickAxis[TURN_LEFT_RIGHT]) > GAMEPAD_CUTOFF) {
        rotationVector.y = joystickAxis[TURN_LEFT_RIGHT] - joystickCalibration[1].y;
    } else rotationVector.y = 0.0f;
    if (fabs(joystickAxis[TURN_UP_DOWN]) > GAMEPAD_CUTOFF) {
        rotationVector.y -= joystickAxis[TURN_UP_DOWN] - joystickCalibration[1].z;
    } else rotationVector.y = 0.0f;


    mModel.rotation.y = 0.0f; //TODO WANJA, RECHNE MAL BITTE DIE TRIGONOMETRIE ZUSAMMEN
    // map 2 werte zwischen -1 und 1 für x und y auf einen rotationswert
    mModel.position += movementVector*PLAYER_MAXSPEED;

}
