#include "Model.h"
#include "util.h"

#include "stdio.h"

Model::Model(std::vector<unsigned short> indices,
        std::vector<glm::vec3> vertices,
        std::vector<glm::vec2> uvs,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec3> tangents,
        std::vector<glm::vec3> bitangents) {

    if (uvs.size() == 0) uvs.reserve(vertices.size());
    if (tangents.size() == 0) tangents.reserve(vertices.size());
    if (bitangents.size() == 0) bitangents.reserve(vertices.size());

    createFromBuffer(indices, vertices, uvs, normals, tangents, bitangents);

}

void Model::createFromBuffer(std::vector<unsigned short> indices,
        std::vector<glm::vec3> vertices,
        std::vector<glm::vec2> uvs,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec3> tangents,
        std::vector<glm::vec3> bitangents) {

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof (glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof (glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof (glm::vec3), &normals[0], GL_STATIC_DRAW);


    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof (glm::vec3), &tangents[0], GL_STATIC_DRAW);


    glGenBuffers(1, &bitangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof (glm::vec3), &bitangents[0], GL_STATIC_DRAW);

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

Model::Model(const char* path) {
    printf("Loading Model 0  from %s\n", path);


    // Read .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;
    bool res = loadModelFromFile(path, indices, vertices, uvs, normals, tangents, bitangents);

    if (!res) {
        printf("Could not load model %s\n", path);
        exit(1);
    }
    printf("success in loadModelFromPath!\n");

    createFromBuffer(indices, vertices, uvs, normals, tangents, bitangents);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // VAA 1 Normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // VAA 2 UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // VAA 3 Tangents
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    // VAA 4 Bitangents
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, (void*) 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}
