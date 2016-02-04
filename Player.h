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
    Player();
    Player(int number);
    virtual void update();
    int playerNumber;
    PointLight* sight;
    PointLight* hookSight;
    PointLight* canonSight;
    Hook* hook;
    Chain* chain;
    void pull();
    bool pulling;
    float health;
    glm::vec3 hookpoint;
    bool fire = false;
    HealthBar* healthBar;
private:
    double lastHookTime;

};

class KeyboardPlayer : public Player {
public:

    KeyboardPlayer(int number) : Player(number) {
    }
    virtual void update();
};

class JoystickPlayer : public Player {
public:

    JoystickPlayer(int number) : Player(number) {
    }
    void calibrate();
    virtual void update();
    const float* joystickAxis;
    int joystickAxisCount;
    const unsigned char* joystickButtons;
    int joystickButtonsCount;
    std::string CONTROLER_NAME;
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
