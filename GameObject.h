#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>

#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "LightSources.h"

class GameObject {
public:
    GameObject();
    GameObject(const GameObject* go);
    GameObject(const char* file);
    Model mModel;
    virtual void update();
    void renderShadow(GLuint shaderID, glm::mat4 MVP);
    virtual void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights, glm::mat4 shadowMVP);
    void kill();
    glm::vec3 rotationVector;
    glm::vec3 movementVector;
    float radius;
};

#endif
