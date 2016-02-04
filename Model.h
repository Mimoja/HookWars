#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "Texture.h"


class Model {
public:

    Model() {}
    Model(const char* file);
    Model(std::vector<unsigned short>  indices,
        std::vector<glm::vec3>  vertices,
        std::vector<glm::vec2>  uvs,
        std::vector<glm::vec3>  normals,
        std::vector<glm::vec3>  tangents,
        std::vector<glm::vec3>  bitangents);
    glm::vec3 rotation, scaling, position;
    glm::mat4 getMatr();
    void render();
    Texture* diffuseTexture = 0;
    Texture* normalTexture = 0;
    Texture* specularTexture = 0;
    Texture* ssaoTexture = 0;
    glm::vec3 max;
    glm::vec3 min;
private:
    void createFromBuffer(std::vector<unsigned short>  indices,
        std::vector<glm::vec3>  vertices,
        std::vector<glm::vec2>  uvs,
        std::vector<glm::vec3>  normals,
        std::vector<glm::vec3>  tangents,
        std::vector<glm::vec3>  bitangents);
    GLuint indexBuffer;
    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint uvBuffer;
    GLuint tangentBuffer;
    GLuint bitangentBuffer;
    GLuint VertexArrayID;
    int indicesCount;

    glm::mat4 sum = glm::mat4(1.0f);

};
#endif
