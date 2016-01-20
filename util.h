#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include "GameObject.h"

void FPS_init(float delay);
void FPS_count();
bool loadModelFromFile( const char * path,
                        std::vector<unsigned short> & indices,
                        std::vector<glm::vec3> & vertices,
                        std::vector<glm::vec2> & uvs,
                        std::vector<glm::vec3> & normals);
glm::vec3 getNavigationEntry(glm::vec3 position);
glm::vec3 circleCollision(glm::vec3 center, float radius, float samples);
glm::vec3 slideAlong(glm::vec3 a, glm::vec3 n);
bool isColliding(GameObject o1, GameObject o2);
#endif
