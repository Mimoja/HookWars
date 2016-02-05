#ifndef LIGHTSOURCES_H
#define LIGHTSOURCES_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct BaseLight {
    glm::vec3 lightColor;
    float intensity;

    BaseLight() {
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    }
};

struct SpecularLight {
    int intensity;
    int power;
};

struct Attenuation {
    float constant;
    float linear;
    float exponential;

    Attenuation() {
        constant = 0.0f;
        linear = 0.0f;
        exponential = 1.0f;
    }

};

struct DirectionLight : public BaseLight {
    glm::vec3 direction;
    SpecularLight specular;
};

struct PointLight : public BaseLight {
    glm::vec3 position;
    Attenuation falloff;
    SpecularLight specular;
};

struct SpotLight : public PointLight {
    glm::vec3 direction;
    float cutoff;
    float hardness;

    SpotLight() {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        cutoff = 0.0f;
        hardness = 0.0f;
    }
};

struct Lights {
    BaseLight ambient;
    std::vector<DirectionLight*> directionalLights;
    std::vector<PointLight*> pointLights;
    std::vector<SpotLight*> spotLights;
};



#endif /* LIGHTSOURCES_H */

