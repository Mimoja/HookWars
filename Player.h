#include "GameObject.h"
#include "Hook.h"
#include <string>

#pragma once

class Player : public GameObject{
	public:
		Player();
		Player(const char* file);
		virtual void update();
		int playerNumber;
		std::string CONTROLER_NAME;
		void calibrate();
		const float* joystickAxis;
		int joystickAxisCount;
		const unsigned char* joystickButtons;
		int joystickButtonsCount;
		PointLight* sight;
		Hook* hook;
    private:
        double lastHookTime;
        glm::vec3 joystickCalibration[2];
};
