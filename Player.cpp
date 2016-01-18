#include "Player.h"
#include "config.h"
#include "stdio.h"
#include "Hook.h"

extern std::vector<unsigned char> navigationMap;
extern unsigned navigationMapHeight;
extern unsigned navigationMapWidth;
extern GameObject* map_ptr;

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

    
    // TODO Map NavigationMap to map_ptr->mModel.min and map_ptr->mModel.max
    // TODO check collision
    //printf("Navigation RGB: %d %d %d\n",navigationMap[0],navigationMap[1],navigationMap[2]);
    //printf("Model reaches from (%f,%f,%f) to (%f,%f,%f) \n",map_ptr->mModel.min.x,map_ptr->mModel.min.y,map_ptr->mModel.min.z,map_ptr->mModel.max.x,map_ptr->mModel.max.y,map_ptr->mModel.max.z);
    mModel.position += movementVector*PLAYER_MAXSPEED;
    mModel.rotation.y   = glm::atan( rotationVector.x,rotationVector.y )+PLAYER_BASE_ROTATION;

    // Fire
    double now  = glfwGetTime();
    if (joystickAxis[FIRE] > 0 
        && now - lastHookTime > HOOK_COOLDOWN) {
        lastHookTime = now;
        printf("Hook fired\n");
        //TODO Spawn Hook
    }

}
