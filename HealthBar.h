
#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Player.h" 

class Player;

class HealthBar : public GameObject {
public:
    HealthBar(Player * o);
    virtual void update();
    virtual void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights, Camera shadowCamera);
private:
    Player* origin = 0;
};

#endif /* HEALTHBAR_H */

