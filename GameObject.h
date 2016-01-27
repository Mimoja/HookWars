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


class GameObject{
    public:
        GameObject();
        GameObject(const char* file);
        Model mModel; 
        virtual void update();
        BoundingBox mBox;
        void renderDiffuse(GLuint shaderID, glm::mat4 MVP);
        void renderNormals(GLuint shaderID, glm::mat4 MVP);
        void renderShadow(GLuint shaderID, glm::mat4 MVP);
        virtual void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights);
        void kill();
        glm::vec3 rotationVector;
        glm::vec3 movementVector;
        glm::vec3 color;
        bool useColor;
        float radius;
    private:
        GLuint lastShader = 1024;
        GLuint WorldMatrixID;
        GLuint ModelMatrixID;
        GLuint CameraPositionID;
        GLuint AmbientLightColorID;
        GLuint AmbientLightIntensityID;
};

#endif
