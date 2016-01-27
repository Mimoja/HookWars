#include "GameObject.h"

#include <stdio.h>
#include <string>

GameObject::GameObject(const char* path) : mModel(path) {
    printf("Building GameObject from %s\n", path);
}

GameObject::GameObject() {
}

void GameObject::update() {

}
void GameObject::renderNormals(GLuint shaderID, glm::mat4 MVP){
    
    // Use our shader
    glUseProgram(shaderID);

    GLuint WorldMatrixID = glGetUniformLocation(shaderID, "WORLD");
    GLuint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");

    // Transformations Matricies
    glm::mat4 modelMatrix = mModel.getMatr();
    glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(modelMatrix[0][0]));

    if (mModel.normalTexture != 0) {
        mModel.normalTexture->bindToUnit(GL_TEXTURE1);
        GLuint normalTextureID = glGetUniformLocation(shaderID, "normal");
        glUniform1i(normalTextureID, 1);
        GLuint useNormalID = glGetUniformLocation(shaderID, "useNormalTexture");
        glUniform1d(useNormalID, 1);
    }
    mModel.render();
}

void GameObject::renderDiffuse(GLuint shaderID, glm::mat4 MVP) {

    // Use our shader
    glUseProgram(shaderID);

    GLuint WorldMatrixID = glGetUniformLocation(shaderID, "WORLD");
    GLuint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");

    // Transformations Matricies
    glm::mat4 modelMatrix = mModel.getMatr();
    glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(modelMatrix[0][0]));

    if (mModel.diffuseTexture != 0) {
        mModel.diffuseTexture->bindToUnit(GL_TEXTURE0);
        GLuint diffuseTextureID = glGetUniformLocation(shaderID, "diffuse");
        glUniform1i(diffuseTextureID, 0);
    }
    mModel.render();
}

void GameObject::renderShadow(GLuint shaderID, glm::mat4 MVP){
    
    // Use our shader
    glUseProgram(shaderID);

    GLuint LightMatrixID = glGetUniformLocation(shaderID, "LIGHT");
    GLuint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");
    
    // Transformations Matricies
    glm::mat4 modelMatrix = mModel.getMatr();
    glUniformMatrix4fv(LightMatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(modelMatrix[0][0]));
    
    mModel.render();
}
void GameObject::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights) {

    // Use our shader
    glUseProgram(shaderID);

    if (shaderID != lastShader) {


        CameraPositionID = glGetUniformLocation(shaderID, "CAMERA");
        AmbientLightColorID = glGetUniformLocation(shaderID, "ambientLight.Color");
        AmbientLightIntensityID = glGetUniformLocation(shaderID, "ambientLight.Intensity");

    } else lastShader = shaderID;

    // Transformations Matricies
    glm::mat4 modelMatrix = mModel.getMatr();
    glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(modelMatrix[0][0]));
    glUniform3f(CameraPositionID, camera.mPos.x, camera.mPos.y, camera.mPos.z);

    // Color
    if (useColor) {
        GLuint colorID = glGetUniformLocation(shaderID, "teamColor");
        GLuint useColorID = glGetUniformLocation(shaderID, "useTeamColor");
        glUniform3f(colorID, color.r, color.g, color.b);
        glUniform1d(useColorID, 1);
    }

    // Textures
    if (mModel.diffuseTexture != 0) {
        mModel.diffuseTexture->bindToUnit(GL_TEXTURE0);
        GLuint diffuseID = glGetUniformLocation(shaderID, "diffuseTextureSampler");
        glUniform1i(diffuseID, 0);
    }

    if (mModel.normalTexture != 0) {
        mModel.normalTexture->bindToUnit(GL_TEXTURE1);
        GLuint normalID = glGetUniformLocation(shaderID, "normalTextureSampler");
        glUniform1i(normalID, 1);
    }

    if (mModel.specularTexture != 0) {
        mModel.specularTexture->bindToUnit(GL_TEXTURE2);
        GLuint specularID = glGetUniformLocation(shaderID, "specularTextureSampler");
        glUniform1i(specularID, 2);
    }

    // Lights
    // Ambient
    glUniform3f(AmbientLightColorID,
            lights.ambient.lightColor.r,
            lights.ambient.lightColor.g,
            lights.ambient.lightColor.b);
    glUniform1f(AmbientLightIntensityID, lights.ambient.intensity);


    // Directional Lights
    GLuint DirectionalLightCountID = glGetUniformLocation(shaderID, "directionalLightCount");
    glUniform1i(DirectionalLightCountID, lights.directionalLights.size());

    for (unsigned int i = 0; i < lights.directionalLights.size(); i++) {

        DirectionLight light = *lights.directionalLights[i];
        std::string lightName = "directionalLight[" + std::to_string(i) + "]";

        GLuint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLuint directionID = glGetUniformLocation(shaderID, (lightName + ".Direction").c_str());
        glUniform3f(directionID, light.direction.x,
                light.direction.y,
                light.direction.z);

        GLuint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        glUniform1f(intensityID, light.intensity);

        GLuint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        glUniform1f(specIntensityID, light.specular.intensity);

        GLuint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        glUniform1i(specPowerID, light.specular.power);
    }

    // Point Lights
    GLuint PointLightCountID = glGetUniformLocation(shaderID, "pointLightCount");
    glUniform1i(PointLightCountID, lights.pointLights.size());

    for (unsigned int i = 0; i < lights.pointLights.size(); i++) {

        PointLight light = *(lights.pointLights[i]);
        std::string lightName = "pointLight[" + std::to_string(i) + "]";

        GLuint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLuint positionID = glGetUniformLocation(shaderID, (lightName + ".Position").c_str());
        glUniform3f(positionID, light.position.x,
                light.position.y,
                light.position.z);

        GLuint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        glUniform1f(intensityID, light.intensity);

        GLuint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        glUniform1f(specIntensityID, light.specular.intensity);

        GLuint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        glUniform1i(specPowerID, light.specular.power);

        GLuint falloffConstantID = glGetUniformLocation(shaderID, (lightName + ".Falloff.constant").c_str());
        glUniform1f(falloffConstantID, light.falloff.constant);

        GLuint falloffLinearID = glGetUniformLocation(shaderID, (lightName + ".Falloff.linear").c_str());
        glUniform1f(falloffLinearID, light.falloff.linear);

        GLuint falloffExponentialID = glGetUniformLocation(shaderID, (lightName + ".Falloff.exponential").c_str());
        glUniform1f(falloffExponentialID, light.falloff.exponential);

    }

    // Spot Lights
    GLuint SpottLightCountID = glGetUniformLocation(shaderID, "spotLightCount");
    glUniform1i(SpottLightCountID, lights.pointLights.size());

    for (unsigned int i = 0; i < lights.spotLights.size(); i++) {

        SpotLight light = *(lights.spotLights[i]);
        std::string lightName = "spotLight[" + std::to_string(i) + "]";

        GLuint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLuint positionID = glGetUniformLocation(shaderID, (lightName + ".Position").c_str());
        glUniform3f(positionID, light.position.x,
                light.position.y,
                light.position.z);

        GLuint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        glUniform1f(intensityID, light.intensity);

        GLuint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        glUniform1f(specIntensityID, light.specular.intensity);

        GLuint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        glUniform1i(specPowerID, light.specular.power);

        GLuint falloffConstantID = glGetUniformLocation(shaderID, (lightName + ".Falloff.constant").c_str());
        glUniform1f(falloffConstantID, light.falloff.constant);

        GLuint falloffLinearID = glGetUniformLocation(shaderID, (lightName + ".Falloff.linear").c_str());
        glUniform1f(falloffLinearID, light.falloff.linear);

        GLuint falloffExponentialID = glGetUniformLocation(shaderID, (lightName + ".Falloff.exponential").c_str());
        glUniform1f(falloffExponentialID, light.falloff.exponential);

        GLuint directionID = glGetUniformLocation(shaderID, (lightName + ".Direction").c_str());
        glUniform3f(directionID, light.direction.x,
                light.direction.y,
                light.direction.z);
        GLuint cutoffID = glGetUniformLocation(shaderID, (lightName + ".Cutoff").c_str());
        glUniform1f(cutoffID, light.cutoff);

        GLuint hardnessID = glGetUniformLocation(shaderID, (lightName + ".Hardness").c_str());
        glUniform1f(hardnessID, light.hardness);

    }

    mModel.render();
}
