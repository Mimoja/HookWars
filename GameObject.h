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
        void update();
        BoundingBox mBox;
        void render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights);
        void kill();
    private:
        GLuint lastShader;
};

#endif
