#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>
#include <vector>

#include "Model.h"
#include "BoundingBox.h"

class GameObject{
    Model mModel;
    void init();
    void update();
    glm::vec3 position, rotation, scaling;
    BoundingBox mBox;
    std::vector<GLuint> mShaderIDs;
    void render(GLuint shader);
    void kill();
};

#endif
