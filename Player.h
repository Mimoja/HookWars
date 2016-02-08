#pragma once

#include "GameObject.h"
#include "Hook.h"
#include "Chain.h"
#include "HealthBar.h"
#include <string>

class Hook;
class Chain;
class HealthBar;

class Player : public GameObject {
    public:
        int playerNumber;
        PointLight* sight;
        PointLight* hookSight;
        PointLight* canonSight;
        Hook* hook;
        Chain* chain;
        bool pulling;
        float health;
        glm::vec3 hookpoint;
        bool fire = false;
        bool grapple = false;
        HealthBar* healthBar;

        Player();
        Player(int number);
        void pull();
	    void hit();
        bool isHit();
        virtual void update();

    private:
        double lastHookTime;
        double lastHitTime;
        double lastGrappleTime;

        void die();
};

class KeyboardPlayer : public Player {
    public:

        KeyboardPlayer(int number) : Player(number) {
        }
        virtual void update();
    private:
        float rotation;
};

class JoystickPlayer : public Player {
    public:
        const float* joystickAxis;
        int joystickAxisCount;
        const unsigned char* joystickButtons;
        int joystickButtonsCount;
        std::string CONTROLER_NAME;

        JoystickPlayer(int number) : Player(number) {}
        void calibrate();
        virtual void update();

    private:
        glm::vec3 joystickCalibration[2];
};

class Rotor : public GameObject {
    public:
        Rotor(Player* owner, float rotation, float height);
        virtual void update();

    private:
        Player* player;
        float rot;
};
