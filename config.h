#ifndef CONFIG_H
#define CONFIG_H

// PROG_NAME comes from makefile
#define GAME_NAME "Hook Wars"

#define PLAYER_MODEL "assets/player.obj"
#define ROTOR_MODEL "assets/rotor.obj"
#define PLAYER_SCALING 1
#define PLAYER_BASE_ROTATION (glm::pi<float>())
#define PLAYER_RADIUS 1.0f

#define HEALTHBAR_MODEL "assets/health.obj"
#define HEALTHBAR_SCALING 0.1f
#define HEALTHBAR_LENGTH 3.0f
#define HEALTHBAR_HEIGHT 4.0f

#define HOOK_MODEL "assets/hook.obj"
#define HOOK_GRAPPLE_MODEL "assets/grapple.obj"
#define HOOK_SCALING 0.4f
#define HOOK_SPEED 0.5f
#define HOOK_RADIUS 0.4f
#define HOOK_BASE_ROTATION glm::half_pi<float>()

#define CHAIN_MODEL "assets/chain_simple.obj"
#define CHAIN_SCALING 0.12f
#define CHAIN_DISTANCE 0.55f
#define CHAIN_BASE_ROTATION glm::pi<float>()
#define CHAIN_BASE_PUSH 0.2f
#define CHAIN_PULL 0.5f
#define CHAIN_BASE_PULL 0.2f

//#define loadCube
#define MAP_MODEL "assets/map1.obj"
#define MAP_SCALING 4.0f
#define MAP_DIFFUSE "assets/T_Tile_Sandstone_02_4096_D.png"
#define MAP_NORMAL "assets/T_Tile_Sandstone_02_4096_N.png"
#define MAP_AO "assets/T_Tile_Sandstone_02_4096_AO.png"

#define NAVIGATION_MAP "assets/Navigation.png"

#define WINDOW_NAME  GAME_NAME
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define LINK_DISTANCE (0.1f)

#define HOOK_COOLDOWN (1.500f) //seconds
#define HOOK_LIFETIME (6.000f) //seconds
#define HOOK_RETRACT_TIME (0.300f) //seconds
#define GAMEPAD_CUTOFF (0.15f)
#define PLAYER_MAXSPEED (0.20f)

// Shading
#define GEOMETRIE_VERTEX "shader/geometrie.vs"
#define GEOMETRIE_FRAGMENT "shader/geometrie.fs"

#define HEALTHBAR_VERTEX "shader/health.vs"
#define HEALTHBAR_FRAGMENT "shader/health.fs"

#define SHADOW_VERTEX "shader/shadow.vs"
#define SHADOW_FRAGMENT "shader/shadow.fs"

namespace XBOX_ONE_GAMEPAD {

    enum {
        TURN_LEFT_RIGHT = 3,
        TURN_UP_DOWN = 4,
        MOVE_LEFT_RIGHT = 0,
        MOVE_UP_DOWN = 1,
        FIRE = 5,
		GRAPPLE = 2,
    };
}

#endif
