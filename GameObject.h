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

class GameObject{
    public:
        GameObject(char* file);
        Model mModel;
        void update();
        glm::vec3 position, rotation, scaling;
        BoundingBox mBox;
        void render(GLuint shader, glm::mat4 MVP, Camera camera);
        void kill();
};

#endif
