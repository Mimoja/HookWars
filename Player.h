#include "GameObject.h"
#include <string>

class Player : public GameObject{
public:
    Player();
    Player(const char* file): GameObject(file){}
    virtual void update();
    int playerNumber;
    std::string CONTROLER_NAME;
    void calibrate();
    const float* joystickAxis;
    int joystickAxisCount;
    const unsigned char* joystickButtons;
    int joystickButtonsCount;
    PointLight* sight;
    private:
        double lastHookTime;
        glm::vec3 joystickCalibration[2];
};
