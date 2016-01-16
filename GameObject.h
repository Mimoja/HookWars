#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>

#include "Model.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "Shader.h"
#include "LightSources.h"
#include "ShadowMap.h"

class GameObject{
    public:
        GameObject();
        GameObject(const char* file);
        Model mModel; 
        virtual void update();
        BoundingBox mBox;
        virtual void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights);
        void renderToShadowMap(GLuint shaderID, glm::mat4 MVP, ShadowMap map);
        void kill();
        glm::vec3 rotationVector;
        glm::vec3 movementVector;
    private:
        GLuint lastShader = 1024;
        GLuint WorldMatrixID;
        GLuint ModelMatrixID;
        GLuint CameraPositionID;
        GLuint AmbientLightColorID;
        GLuint AmbientLightIntensityID;
};

#endif
