#ifndef CONFIG_H
#define CONFIG_H

// PROG_NAME comes from makefile
#define GAME_NAME "Hook Wars"

#define PLAYER_MODEL "assets/derp.obj"
#define PLAYER_SCALING 3

#define MAP_MODEL "assets/map1.obj"
#define MAP_SCALING 1

#define WINDOW_NAME  GAME_NAME
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define LINK_DISTANCE (0.1f)

#define HOOK_COOLDOWN (0.300f); //seconds
#define PLAYER_MAXSPEED (0.50f)
#define GAMEPAD_CUTOFF (0.15f)

enum XBOX_ONE_GAMEPAD{
    TURN_LEFT_RIGHT = 0,
    TURN_UP_DOWN = 1,
    MOVE_LEFT_RIGHT = 3,
    MOVE_UP_DOWN = 4,
    FIRE = 5,
};

enum PlayerColor {
    blue = 1,
    red  = 2,
};

#endif
