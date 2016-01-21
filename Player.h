#pragma once

#include "GameObject.h"
#include "Hook.h"
#include "Chain.h"
#include <string>

class Hook;
class Chain;

class Player : public GameObject{
	public:
		Player();
		Player(const char* file);
		virtual void update();
		virtual void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights);
		int playerNumber;
		std::string CONTROLER_NAME;
		void calibrate();
		const float* joystickAxis;
		int joystickAxisCount;
		const unsigned char* joystickButtons;
		int joystickButtonsCount;
		PointLight* sight;
		Hook* hook;
		Chain* chain;
		bool pulling;
    private:
        double lastHookTime;
        glm::vec3 joystickCalibration[2];
};
