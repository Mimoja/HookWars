#ifndef CONFIG_H
#define CONFIG_H

// PROG_NAME comes from makefile
#define GAME_NAME "Hook Wars"

#define PLAYER_MODEL "assets/bunny_blend.obj"
#define PLAYER_SCALING 25

#define MAP_MODEL "assets/map1.obj"
#define MAP_SCALING 1

#define WINDOW_NAME  GAME_NAME
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define LINK_DISTANCE (0.1f)

enum PlayerColor {
    blue = 1,
    red  = 2,
};

#endif
