#include "Model.h"
#include "util.h"

#include "stdio.h"

Model::Model(const char* path) {
    printf("Loading Model 0  from %s\n", path);
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Read .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadModelFromFile(path, indices, vertices, uvs, normals);

    if (!res) {
        printf("Could not load model %s\n", path);
        exit(1);
    }
    printf("success in loadModelFromPath!\n");

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof (glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof (glm::vec3), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof (unsigned short), &indices[0], GL_STATIC_DRAW);

    indicesCount = indices.size();
    printf("Model has %d indices\n", indicesCount);

    for (glm::vec3 v : vertices) {
        if (v.x > max.x)max.x = v.x;
        if (v.y > max.y)max.y = v.y;
        if (v.z > max.z)max.z = v.z;
        if (v.x < min.x)min.x = v.x;
        if (v.y < min.y)min.y = v.y;
        if (v.z < min.z)min.z = v.z;
    }
    printf("Model reaches from (%f,%f,%f) to (%f,%f,%f) \n", min.x, min.y, min.z, max.x, max.y, max.z);
    scaling = glm::vec3(1.0f);
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);

}

glm::mat4 Model::getMatr() {
    sum = glm::mat4(1.0f);
    sum = glm::translate(sum, position);
    sum = glm::scale(sum, scaling);
    sum = glm::rotate(sum, rotation[0], glm::vec3(1, 0, 0));
    sum = glm::rotate(sum, rotation[1], glm::vec3(0, 1, 0));
    sum = glm::rotate(sum, rotation[2], glm::vec3(0, 0, 1));


    return sum;
}

void Model::render() {

    glBindVertexArray(VertexArrayID);

    // VAA 0 Verticies
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0, // attribute
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*) 0 // array buffer offset
        );

    // VAA 1 Normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        1, // attribute
        3, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*) 0 // array buffer offset
        );

    // VAA 2 UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
        2, // attribute
        2, // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0, // stride
        (void*) 0 // array buffer offset
        );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES, // mode
        indicesCount, // count
        GL_UNSIGNED_SHORT, // type
        (void*) 0 // element array buffer offset
        );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
