#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "ShadowMap.h"

class Model {
public:

    Model() {}
    Model(const char* file);
    glm::vec3 rotation, scaling, position;
    glm::mat4 getMatr();
    void render();
    Texture* diffuseTexture = 0;
    Texture* normalTexture = 0;
    Texture* specularTexture = 0;
    ShadowMap* shadowMap = 0;
    glm::vec3 max;
    glm::vec3 min;
private:
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint uvBuffer;
    GLuint VertexArrayID;
    int indicesCount;

    glm::mat4 sum = glm::mat4(1.0f);

};
#endif
