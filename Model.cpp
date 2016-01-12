#include "Model.h"
#include "util.h"

#include "stdio.h"

void Model::readFile(const char* path){
    printf("Loading Model 0  from %s\n",path);
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Read .obj file
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool res = loadModelFromFile(path, indices, vertices, uvs, normals);

    if(!res){
        printf("Could not load model %s\n",path);
        exit(1);
    }
     printf("success!\n");

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

    indicesCount = indices.size();
    printf("Model has %d indice\n", indicesCount);

}

void Model::setPosition(float x,float y,float z){
    translation=glm::vec3(x,y,z);
}
void Model::setRotation(float x, float y, float z){
    rotation=glm::vec3(x,y,z);
}
void Model::setScaling(float x, float y, float z){
    scaling=glm::vec3(x,y,z);
}
glm::mat4 Model::getMatr(){
    sum=glm::mat4(1.0f);
    sum=glm::scale(sum,scaling);
    sum=glm::rotate(sum,rotation[0],glm::vec3(1,0,0));
    sum=glm::rotate(sum,rotation[1],glm::vec3(0,1,0));
    sum=glm::rotate(sum,rotation[2],glm::vec3(0,0,1));

    sum=glm::translate(sum,translation);
    return sum;
}

void Model::render(){

    glBindVertexArray(VertexArrayID);

    // VAA 0 Verticies
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // VAA 1 Normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
        1,                                // attribute
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // VAA 2 UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
        2,                                // attribute
        2,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // Draw the triangles !
    glDrawElements(
        GL_TRIANGLES,                     // mode
        indicesCount,                     // count
        GL_UNSIGNED_SHORT,                // type
        (void*)0                          // element array buffer offset
    );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
