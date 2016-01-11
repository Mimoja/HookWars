#ifndef UTIL_H
#define UTIL_H

#include <vector>

void FPS_init(float delay);
void FPS_count();
bool loadModelFromFile( const char * path, 
                        std::vector<unsigned short> & indices,
                        std::vector<glm::vec3> & vertices,
                        std::vector<glm::vec2> & uvs,
                        std::vector<glm::vec3> & normals);
#endif
