#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>


typedef struct _AmbientLight{
    glm::vec3 lightColor;
    float intensity;
} AmbientLight;

typedef struct _SpecularLight{
    float intensity;
} SpecularLight;

typedef struct _DirectionLight{
    glm::vec3 lightColor;
    glm::vec3 position;
    float intensity;
    glm::vec3 direction;
    float falloff;
    SpecularLight specular;
} DirectionLight;

typedef struct _PointLight{
    glm::vec3 lightColor;
    glm::vec3 position;
    float intensity;
    float falloff;
    SpecularLight specular;
} PointLight;

typedef struct _SpotLight{
    glm::vec3 lightColor;
    glm::vec3 position;
    glm::vec3 direction;
    float intensity;
    float falloff;
    SpecularLight specular;
} SpotLight;

typedef struct _Lights{
    AmbientLight ambient;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight>  spotLights;
} Lights;



#endif /* LIGHTSOURCES_H */

