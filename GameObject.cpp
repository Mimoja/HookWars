#include "GameObject.h"

#include <stdio.h>
#include <string>

GameObject::GameObject(const char* path) : mModel(path) {
    printf("Building GameObject from %s\n", path);
}

GameObject::GameObject() {
}

GameObject::GameObject(const GameObject & go){
    rotationVector = go.rotationVector;
    movementVector = go.movementVector;
    radius = go.radius;
    mModel = go.mModel;
}

void GameObject::update() {

}

void GameObject::renderShadow(GLuint shaderID, glm::mat4 MVP) {

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

void GameObject::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights, Camera shadowCamera) {

    // Use our shader
    glUseProgram(shaderID);

    GLint WorldMatrixID = glGetUniformLocation(shaderID, "WORLD");
    GLint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");
    GLint ShadowMatrixID = glGetUniformLocation(shaderID, "SHADOW");
    GLint CameraPositionID = glGetUniformLocation(shaderID, "CAMERA");
    GLint AmbientLightColorID = glGetUniformLocation(shaderID, "ambientLight.Color");
    GLint AmbientLightIntensityID = glGetUniformLocation(shaderID, "ambientLight.Intensity");

    // Transformations Matricies
    glm::mat4 modelMatrix = mModel.getMatr();
    if (WorldMatrixID != -1)glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
    if (ModelMatrixID != -1)glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(modelMatrix[0][0]));
    if (ShadowMatrixID != -1)glUniformMatrix4fv(ShadowMatrixID, 1, GL_FALSE, &(shadowCamera.getView()[0][0]));
    if (CameraPositionID != -1)glUniform3f(CameraPositionID, camera.mPos.x, camera.mPos.y, camera.mPos.z);

    // Textures
    if (mModel.diffuseTexture != 0) {
        mModel.diffuseTexture->bindToUnit(GL_TEXTURE0);
        GLint diffuseID = glGetUniformLocation(shaderID, "DiffuseTextureSampler");
        if (diffuseID != -1)glUniform1i(diffuseID, 0);
    }

    if (mModel.normalTexture != 0) {
        mModel.normalTexture->bindToUnit(GL_TEXTURE1);
        GLint normalID = glGetUniformLocation(shaderID, "NormalTextureSampler");
        if (normalID != -1)glUniform1i(normalID, 1);
    }

    if (mModel.specularTexture != 0) {
        mModel.specularTexture->bindToUnit(GL_TEXTURE2);
        GLint specularID = glGetUniformLocation(shaderID, "SpecularTextureSampler");
        if (specularID != -1)glUniform1i(specularID, 2);
    }

    if (mModel.ssaoTexture != 0) {
        mModel.ssaoTexture->bindToUnit(GL_TEXTURE3);
        GLint ssaoID = glGetUniformLocation(shaderID, "SSAOTextureSampler");
        if (ssaoID != -1)glUniform1i(ssaoID, 3);
    }


    extern GLuint shadowTexture;
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);
    GLint shadowID = glGetUniformLocation(shaderID, "ShadowTextureSampler");
    if (shadowID != -1)glUniform1i(shadowID, 4);


    // Lights
    // Ambient
    if (AmbientLightColorID != -1)glUniform3f(AmbientLightColorID,
            lights.ambient.lightColor.r,
            lights.ambient.lightColor.g,
            lights.ambient.lightColor.b);
    if (AmbientLightIntensityID != -1)glUniform1f(AmbientLightIntensityID, lights.ambient.intensity);


    // Directional Lights
    GLint DirectionalLightCountID = glGetUniformLocation(shaderID, "directionalLightCount");
    if (DirectionalLightCountID != -1)glUniform1i(DirectionalLightCountID, lights.directionalLights.size());

    for (unsigned int i = 0; i < lights.directionalLights.size(); i++) {

        DirectionLight light = *lights.directionalLights[i];
        std::string lightName = "directionalLight[" + std::to_string(i) + "]";

        GLint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        if (colorID != -1)glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLint directionID = glGetUniformLocation(shaderID, (lightName + ".Direction").c_str());
        if (directionID != -1)glUniform3f(directionID, light.direction.x,
                light.direction.y,
                light.direction.z);

        GLint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        if (intensityID != -1)glUniform1f(intensityID, light.intensity);

        GLint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        if (specIntensityID != -1)glUniform1f(specIntensityID, light.specular.intensity);

        GLint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        if (specPowerID != -1)glUniform1i(specPowerID, light.specular.power);
    }

    // Point Lights
    GLuint PointLightCountID = glGetUniformLocation(shaderID, "pointLightCount");
    glUniform1i(PointLightCountID, lights.pointLights.size());

    for (unsigned int i = 0; i < lights.pointLights.size(); i++) {

        PointLight light = *(lights.pointLights[i]);
        std::string lightName = "pointLight[" + std::to_string(i) + "]";

        GLint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        if (colorID != -1)glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLint positionID = glGetUniformLocation(shaderID, (lightName + ".Position").c_str());
        if (positionID != -1)glUniform3f(positionID, light.position.x,
                light.position.y,
                light.position.z);

        GLint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        if (intensityID != -1)glUniform1f(intensityID, light.intensity);

        GLint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        if (specIntensityID != -1)glUniform1f(specIntensityID, light.specular.intensity);

        GLint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        if (specPowerID != -1)glUniform1i(specPowerID, light.specular.power);

        GLint falloffConstantID = glGetUniformLocation(shaderID, (lightName + ".Falloff.constant").c_str());
        if (falloffConstantID != -1)glUniform1f(falloffConstantID, light.falloff.constant);

        GLint falloffLinearID = glGetUniformLocation(shaderID, (lightName + ".Falloff.linear").c_str());
        if (falloffLinearID != -1)glUniform1f(falloffLinearID, light.falloff.linear);

        GLint falloffExponentialID = glGetUniformLocation(shaderID, (lightName + ".Falloff.exponential").c_str());
        if (falloffExponentialID != -1)glUniform1f(falloffExponentialID, light.falloff.exponential);

    }

    // Spot Lights
    GLint SpottLightCountID = glGetUniformLocation(shaderID, "spotLightCount");
    if (SpottLightCountID != -1)glUniform1i(SpottLightCountID, lights.pointLights.size());

    for (unsigned int i = 0; i < lights.spotLights.size(); i++) {

        SpotLight light = *(lights.spotLights[i]);
        std::string lightName = "spotLight[" + std::to_string(i) + "]";

        GLint colorID = glGetUniformLocation(shaderID, (lightName + ".Color").c_str());
        if (colorID != -1)glUniform3f(colorID, light.lightColor.r,
                light.lightColor.g,
                light.lightColor.b);

        GLint positionID = glGetUniformLocation(shaderID, (lightName + ".Position").c_str());
        if (positionID != -1)glUniform3f(positionID, light.position.x,
                light.position.y,
                light.position.z);

        GLint intensityID = glGetUniformLocation(shaderID, (lightName + ".Intensity").c_str());
        if (intensityID != -1)glUniform1f(intensityID, light.intensity);

        GLint specIntensityID = glGetUniformLocation(shaderID, (lightName + ".SpecularIntensity").c_str());
        if (specIntensityID != -1)glUniform1f(specIntensityID, light.specular.intensity);

        GLint specPowerID = glGetUniformLocation(shaderID, (lightName + ".SpecularPower").c_str());
        if (specPowerID != -1)glUniform1i(specPowerID, light.specular.power);

        GLint falloffConstantID = glGetUniformLocation(shaderID, (lightName + ".Falloff.constant").c_str());
        if (falloffConstantID != -1)glUniform1f(falloffConstantID, light.falloff.constant);

        GLint falloffLinearID = glGetUniformLocation(shaderID, (lightName + ".Falloff.linear").c_str());
        if (falloffLinearID != -1)glUniform1f(falloffLinearID, light.falloff.linear);

        GLint falloffExponentialID = glGetUniformLocation(shaderID, (lightName + ".Falloff.exponential").c_str());
        if (falloffExponentialID != -1)glUniform1f(falloffExponentialID, light.falloff.exponential);

        GLint directionID = glGetUniformLocation(shaderID, (lightName + ".Direction").c_str());
        if (directionID != -1)glUniform3f(directionID, light.direction.x,
                light.direction.y,
                light.direction.z);
        GLint cutoffID = glGetUniformLocation(shaderID, (lightName + ".Cutoff").c_str());
        if (cutoffID != -1)glUniform1f(cutoffID, light.cutoff);

        GLint hardnessID = glGetUniformLocation(shaderID, (lightName + ".Hardness").c_str());
        if (hardnessID != -1)glUniform1f(hardnessID, light.hardness);

    }

    mModel.render();
}
