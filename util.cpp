
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "util.h"
#include "GameObject.h"
#include "config.h"

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
        float t = (currentTime - lastTime)*1000.0 / double(nbFrames);
        printf("%f ms/frame, %.1f frames / second\n", t, 1000.0f / t);
        nbFrames = 0;
        lastTime = glfwGetTime(); // TODO can we add the delay and save time?
    }
}

void calculateNormals(std::vector<glm::vec3> & vertices,
    std::vector<glm::vec3> & normals,
    std::vector<unsigned short> & indices,
    unsigned int faceCount) {

    assert(indices.size() >= 3 * faceCount);

    normals = std::vector<glm::vec3>(vertices.size());
    for (unsigned int i = 0; i < faceCount; i++) {
        auto i1 = indices[ i * 3 ];
        auto i2 = indices[ i * 3 + 1];
        auto i3 = indices[ i * 3 + 2];

        auto facenormal = glm::cross(
            vertices[i3] - vertices[i1],
            vertices[i2] - vertices[i1]);

        normals[i1] -= facenormal;
        normals[i2] -= facenormal;
        normals[i3] -= facenormal;
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
        printf("Reading %d UVs\n", mesh->mNumUVComponents[0]);
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
        printf("Reading %d Normals\n", mesh->mNumVertices);
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            aiVector3D n = mesh->mNormals[i];
            normals.push_back(glm::vec3(n.x, n.y, n.z));
        }
    } else {
        printf("No Normals Found. Calculating manual\n");
        calculateNormals(vertices, normals, indices, mesh->mNumFaces);
    }
    return true;
}

glm::vec3 getNavigationEntry(glm::vec3 position) {

    extern std::vector<unsigned char> navigationMap;
    extern unsigned navigationMapHeight;
    extern unsigned navigationMapWidth;
    extern GameObject* map_ptr;

    glm::vec3 relativePosition = position - (map_ptr->mModel.min * MAP_SCALING);
    relativePosition /= (map_ptr->mModel.max - (map_ptr->mModel.min)) * MAP_SCALING;

    glm::vec3 rgb(-1.0f);
    if (relativePosition.x < 0 || relativePosition.x > 1
        || relativePosition.z < 0 || relativePosition.z > 1)return rgb;

    unsigned int navPosition = ((int) (relativePosition.z * navigationMapHeight) * navigationMapWidth) + relativePosition.x * navigationMapWidth;
    navPosition *= 4; // RGBA

    rgb.r = navigationMap[navPosition];
    rgb.g = navigationMap[navPosition + 1];
    rgb.b = navigationMap[navPosition + 2];
    rgb /= 255;

    return rgb;
}

glm::vec3 circleCollision(glm::vec3 center, float radius, float samples){
	glm::vec3 normal = glm::vec3(0);
	for (float a = 0; a < 2 * glm::pi<float>(); a += glm::half_pi<float>()/samples){
		glm::vec3 offset = radius * glm::vec3(sin(a), 0, cos(a));
		glm::vec3 navEntry = getNavigationEntry(center + offset);
		if (navEntry.r == -1.0f || navEntry.r == 1.0f) {
        	normal -= offset;
    	}
	}
	return normal;
}
