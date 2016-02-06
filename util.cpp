
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "util.h"
#include "GameObject.h"
#include "config.h"
#include "Player.h"

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

extern std::vector<Player*> allPlayers;

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
        std::vector<glm::vec3> & normals,
        std::vector<glm::vec3> & tangents,
        std::vector<glm::vec3> & bitangents) {

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
    } else {
        uvs = std::vector<glm::vec2>(vertices.size());
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
    printf("Calculating tangents and bitangents\n");

    tangents = std::vector<glm::vec3>(vertices.size());
    bitangents = std::vector<glm::vec3>(vertices.size());
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        auto i1 = indices[ i * 3 ];
        auto i2 = indices[ i * 3 + 1];
        auto i3 = indices[ i * 3 + 2];

        auto v1 = vertices[i1];
        auto v2 = vertices[i2];
        auto v3 = vertices[i3];

        auto uv1 = uvs[i1];
        auto uv2 = uvs[i2];
        auto uv3 = uvs[i3];

        auto d1 = v2 - v1;
        auto d2 = v3 - v1;

        auto dUV1 = uv2 - uv1;
        auto dUV2 = uv3 - uv1;

        float r = 1.0f / (dUV1.x * dUV2.y - dUV1.y * dUV2.x);
        glm::vec3 tangent = (d1 * dUV2.y - d2 * dUV1.y) * r;
        glm::vec3 bitangent = (d2 * dUV1.x - d1 * dUV2.x) * r;

        tangents[i1] = tangent;
        tangents[i2] = tangent;
        tangents[i3] = tangent;

        bitangents[i1] = bitangent;
        bitangents[i2] = bitangent;
        bitangents[i3] = bitangent;
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

glm::vec3 circleCollision(glm::vec3 center, float radius, float samples, bool collideWithGreen,
		bool collideWithPlayers, Player* self) {
    glm::vec3 normal = glm::vec3(0);
    for (float a = 0; a < 2 * glm::pi<float>(); a += glm::half_pi<float>() / samples) {
        glm::vec3 offset = radius * glm::vec3(sin(a), 0, cos(a));
        glm::vec3 navEntry = getNavigationEntry(center + offset);

		if(collideWithPlayers){
			for (auto p : allPlayers) {
				if(p != self && glm::length(center - p->mModel.position) < PLAYER_RADIUS + radius) {
					normal += 0.1f*(center - p->mModel.position);
				}
			}
		}

        if (navEntry.r == -1.0f || navEntry.r == 1.0f || (collideWithGreen && navEntry.g == 1.0f)) {
            normal -= offset;
        }
    }
    return normal;
}

glm::vec3 slideAlong(glm::vec3 a, glm::vec3 n) {
    auto b = glm::vec3(-n.z, 0, n.x);
    auto s = glm::dot(a, glm::normalize(b));
    return b * s;
}

bool isColliding(GameObject o1, GameObject o2) {
	return (glm::length(o1.mModel.position - o2.mModel.position) < o1.radius + o2.radius);
}

glm::vec3 moveTowards(glm::vec3 pos, glm::vec3 target, float minspeed) {
    glm::vec3 dif = pos - target;
    glm::vec3 moveTo = target + glm::normalize(dif) * CHAIN_DISTANCE;

    // if we can't move straight forwards, check for a better place
    glm::vec3 navEntry = getNavigationEntry(moveTo);
    if (navEntry.r == -1.0f || navEntry.r == 1.0f) {
        std::vector<glm::vec3> possiblePlaces;
        for (float a = 0; a < 2 * glm::pi<float>(); a += glm::half_pi<float>() / 4.0f) {
            glm::vec3 offset = CHAIN_DISTANCE * glm::vec3(sin(a), 0, cos(a));
            glm::vec3 navEntry = getNavigationEntry(target + offset);
            if (!(navEntry.r == -1.0f || navEntry.r == 1.0f)) {
                possiblePlaces.push_back(target + offset);
            }
        }
        // we can't be anywhere? just move forwards!
        if (possiblePlaces.size() == 0) {
            possiblePlaces.push_back(target + glm::normalize(dif) * CHAIN_DISTANCE);
        }

        glm::vec3 nearest = moveTo;
        float bestDist = 1337.0f;
        for (auto v : possiblePlaces) {
            float newDist = glm::length(pos - v);
            if (newDist < bestDist) {
                nearest = v;
                bestDist = newDist;
            }
        }
        return nearest;
    } else {
        // just move straight forwards
        float speed;
        if (glm::length(dif) > CHAIN_DISTANCE) {
            speed = glm::length(dif) - CHAIN_DISTANCE;
        } else {
            speed = minspeed;
        }
        return pos + speed * glm::normalize(-dif);
        printf("%f\n", glm::length(pos - target) / CHAIN_DISTANCE);
    }

}
