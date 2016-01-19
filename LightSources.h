#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

typedef struct _BaseLight {
    glm::vec3 lightColor;
    float intensity;

    _BaseLight() {
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }
} BaseLight;

typedef struct _SpecularLight {
    int intensity;
    int power;
} SpecularLight;

typedef struct _Attenuation {
    float constant;
    float linear;
    float exponential;

    _Attenuation() {
        constant = 0.0f;
        linear = 0.0f;
        exponential = 1.0f;
    }

} Attenuation;

typedef struct _DirectionLight : public BaseLight {
    glm::vec3 direction;
    SpecularLight specular;
} DirectionLight;

typedef struct _PointLight : public BaseLight {
    glm::vec3 position;
    Attenuation falloff;
    SpecularLight specular;
} PointLight;

typedef struct _SpotLight : public PointLight {
    glm::vec3 direction;
    float cutoff;
    float hardness;

    _SpotLight() {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        cutoff = 0.0f;
        hardness = 0.0f;
    }
} SpotLight;

typedef struct _Lights {
    BaseLight ambient;
    std::vector<DirectionLight*> directionalLights;
    std::vector<PointLight*> pointLights;
    std::vector<SpotLight*> spotLights;
} Lights;



#endif /* LIGHTSOURCES_H */

