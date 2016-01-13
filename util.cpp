
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "util.h"

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


float del = 2;
double lastTime = 0.0f;
int nbFrames = 0;

void FPS_init(float delay) {
    lastTime = glfwGetTime();
    nbFrames = 0;
    del = delay;
}

void FPS_count() {
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= del) {
        float t = (1000.0 * del) / double(nbFrames);
        printf("%f ms/frame, %.1f frames / second\n", t, 1000.0f / t);
        nbFrames = 0;
        lastTime = glfwGetTime(); // TODO can we add the delay and save time?
    }
}

void calculateNormals( std::vector<glm::vec3> & vertices, 
                       std::vector<glm::vec3> & normals,
                       std::vector<unsigned short> & indices,
                       unsigned int faceCount) {

    assert(indices.size() >= 3*faceCount);

    normals = std::vector<glm::vec3>(vertices.size());
    for (unsigned int i = 0; i < faceCount; i++) {
        auto i1 = indices[ faceCount*3    ];
        auto i2 = indices[ faceCount*3 + 1];
        auto i3 = indices[ faceCount*3 + 2];

        auto facenormal = glm::cross(
            vertices[i3] - vertices[i1],
            vertices[i2] - vertices[i1]);

        normals[i1] += facenormal;
        normals[i2] += facenormal;
        normals[i3] += facenormal;
    }
}

bool loadModelFromFile(const char * path,
    std::vector<unsigned short> & indices,
    std::vector<glm::vec3> & vertices,
    std::vector<glm::vec2> & uvs,
    std::vector<glm::vec3> & normals) {

    Assimp::Importer importer;
    printf("Staring importer\n");

    const aiScene* scene = importer.ReadFile(path, 0);
    if (!scene) {
        printf("%s\n", importer.GetErrorString());
        getchar();
        return false;
    }

    if (scene->mNumMeshes == 0) {
        printf("No mesh found!\n");
        return false;
    }

    printf("success!!\n");
    const aiMesh* mesh = scene->mMeshes[0];

    vertices.reserve(mesh->mNumVertices);
    uvs.reserve(mesh->mNumVertices);
    normals.reserve(mesh->mNumVertices);
    indices.reserve(3 * mesh->mNumFaces);


    printf("Reading %d Verticies\n", (int) mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    if (mesh->HasTextureCoords(0)) {
        printf("Reading %d UVs\n", mesh->mNumUVComponents);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D UVW = mesh->mTextureCoords[0][i];
            uvs.push_back(glm::vec2(UVW.x, UVW.y));
        }
    }
    if (mesh->HasFaces()) {
        printf("Reading %d Faces as Indices\n", mesh->mNumFaces);
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            indices.push_back(mesh->mFaces[i].mIndices[0]);
            indices.push_back(mesh->mFaces[i].mIndices[1]);
            indices.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }
    if (mesh->HasNormals()) {
        printf("Reading Normals\n");
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D n = mesh->mNormals[i];
            normals.push_back(glm::vec3(n.x, n.y, n.z));
        }
    }else{
        calculateNormals(vertices, normals, indices, mesh->mNumFaces);
    }

    return true;
}
