#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera{
public:
    Camera(glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up);
    glm::mat4 getView();
    void setDomain(glm::vec3 p1, glm::vec3 p2);
    void handleKeyboard(GLFWwindow* window);
    float cameraSpeed = 0.5f;
    glm::vec3 mPos=glm::vec3(0.0f,0.0f,0.0f);
private:
    glm::vec3 mD1, mD2;
    glm::vec3 mBasePos=glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 mTarget=glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 mUp=glm::vec3(0.0f,0.0f,0.0f);
};

#endif
