#include "Player.h"
#include "config.h"
#include "stdio.h"

void Player::update() {
    movementVector.x = joystickAxis[MOVE_LEFT_RIGHT];
    movementVector.y = joystickAxis[MOVE_UP_DOWN];
    rotationVector.x = joystickAxis[TURN_LEFT_RIGHT];
    rotationVector.y = joystickAxis[TURN_UP_DOWN];

    mModel.rotation.x += rotationVector.x;
    mModel.position.x += movementVector.x;
    mModel.position.y += movementVector.y;
    mModel.rotation.x += 0.01f;
}
