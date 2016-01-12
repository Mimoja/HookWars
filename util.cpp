
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
    lastTime= glfwGetTime();
    nbFrames = 0;
    del = delay;
}
void FPS_count(){
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= del ){
        float t = (1000.0*del)/double(nbFrames);
        printf("%f ms/frame, %.1f frames / second\n",t,1000.0f/t);
        nbFrames = 0;
        lastTime = glfwGetTime(); // TODO can we add the delay and save time?
    }
}

bool loadModelFromFile( const char * path,
                        std::vector<unsigned short> & indices,
                        std::vector<glm::vec3> & vertices,
                        std::vector<glm::vec2> & uvs,
                        std::vector<glm::vec3> & normals){

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path, 0);
    if( !scene) {
        fprintf( stderr, importer.GetErrorString());
        getchar();
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    vertices.reserve(mesh->mNumVertices);
    uvs.reserve(mesh->mNumVertices);
    normals.reserve(mesh->mNumVertices);
    indices.reserve(3*mesh->mNumFaces);

    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D pos = mesh->mVertices[i];
        vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
    }

    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D UVW = mesh->mTextureCoords[0][i];
        uvs.push_back(glm::vec2(UVW.x, UVW.y));
    }

    for(unsigned int i=0; i<mesh->mNumVertices; i++){
        aiVector3D n = mesh->mNormals[i];
        normals.push_back(glm::vec3(n.x, n.y, n.z));
    }

    for (unsigned int i=0; i<mesh->mNumFaces; i++){
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }
    return true;
}
