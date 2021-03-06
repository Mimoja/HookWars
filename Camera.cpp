#include "Camera.h"
#include "config.h"

#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

Camera::Camera(){
    Camera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f));
}

Camera::Camera(glm::vec3 Pos, glm::vec3 Target, glm::vec3 Up){
    mBasePos = mPos = Pos;
    mTarget = Target;
    mUp = Up;

}
glm::mat4 Camera::getView(){
    return glm::lookAt(mPos,mPos+mTarget,mUp);
}
void Camera::setDomain(glm::vec3 p1, glm::vec3 p2){
    mD1 = p1;
    mD2 = p2;
}

void Camera::handleKeyboard(GLFWwindow* window){

    if(glfwGetKey(window, GLFW_KEY_KP_8)==GLFW_PRESS)
        mPos[2]-=cameraSpeed;

    if(glfwGetKey(window, GLFW_KEY_KP_2)==GLFW_PRESS)
        mPos[2]+=cameraSpeed;


    if(glfwGetKey(window, GLFW_KEY_KP_4)==GLFW_PRESS)
        mPos[0]-=cameraSpeed;


    if(glfwGetKey(window, GLFW_KEY_KP_6)==GLFW_PRESS)
        mPos[0]+=cameraSpeed;

    if(glfwGetKey(window, GLFW_KEY_KP_0)==GLFW_PRESS)
        mPos = mBasePos;
    if(glfwGetKey(window, GLFW_KEY_KP_7)==GLFW_PRESS)
        mPos[1]+=cameraSpeed;
    if(glfwGetKey(window, GLFW_KEY_KP_9)==GLFW_PRESS)
    mPos[1]-=cameraSpeed;
    if(mD1[2]>mD2[2]){
        if(mPos[2]>mD1[2])mPos[2]=mD1[2];
        if(mPos[2]<mD2[2])mPos[2]=mD2[2];
    }else{
        if(mPos[2]<mD1[2])mPos[2]=mD1[2];
        if(mPos[2]>mD2[2])mPos[2]=mD2[2];
    }
    if(mD1[1]>mD2[1]){
        if(mPos[1]>mD1[1])mPos[1]=mD1[1];
        if(mPos[1]<mD2[1])mPos[1]=mD2[1];
    }else{
        if(mPos[1]<mD1[1])mPos[1]=mD1[1];
        if(mPos[1]>mD2[1])mPos[1]=mD2[1];
    }
    if(mD1[0]>mD2[0]){
        if(mPos[0]>mD1[0])mPos[0]=mD1[0];
        if(mPos[0]<mD2[0])mPos[0]=mD2[0];
    }else{
        if(mPos[0]<mD1[0])mPos[0]=mD1[0];
        if(mPos[0]>mD2[0])mPos[0]=mD2[0];
    }

}

