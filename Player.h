#include "GameObject.h"
#include <string>

class Player : public GameObject{
public:
    Player();
    Player(const char* file): GameObject(file){}
    void update();
    int playerNumber;
    std::string CONTROLER_NAME;
    const float* joystickAxis;
    int joystickAxisCount;
    const unsigned char* joystickButtons;
    int joystickButtonsCount;
};
