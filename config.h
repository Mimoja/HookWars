#ifndef CONFIG_H
#define CONFIG_H

// PROG_NAME comes from makefile
#define GAME_NAME "Hook Wars"

#define PLAYER_MODEL "assets/derp.obj"
#define PLAYER_SCALING 1
#define PLAYER_BASE_ROTATION (3*glm::half_pi<float>())

#define HOOK_MODEL "assets/hook.obj"
#define HOOK_SCALING 0.3f
#define HOOK_SPEED 0.2f
#define HOOK_RADIUS 0.1f
#define HOOK_BASE_ROTATION glm::half_pi<float>()

//#define loadCube
#define MAP_MODEL "assets/map1.obj"
#define MAP_SCALING 4.0f

#define NAVIGATION_MAP "assets/Navigation.png"

#define WINDOW_NAME  GAME_NAME
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define LINK_DISTANCE (0.1f)

#define HOOK_COOLDOWN (1.500f) //seconds
#define PLAYER_MAXSPEED (0.20f)
#define GAMEPAD_CUTOFF (0.15f)

enum XBOX_ONE_GAMEPAD{
    TURN_LEFT_RIGHT = 3,
    TURN_UP_DOWN = 4,
    MOVE_LEFT_RIGHT = 0,
    MOVE_UP_DOWN = 1,
    FIRE = 5,
};

enum PlayerColor {
    blue = 1,
    red  = 2,
};

#endif
