#include "GameObject.h"

#include "stdio.h"
#include <string>

GameObject::GameObject(const char* path){
    printf("Building GameObject from %s\n",path);
    mModel.readFile(path);
}

GameObject::GameObject(){}

void GameObject::update(){}

void GameObject::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights){

          // Use our shader
        glUseProgram(shaderID);

        GLuint WorldMatrixID = glGetUniformLocation(shaderID, "WORLD");
        GLuint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");
        GLuint CameraPositionID = glGetUniformLocation(shaderID, "CAMERA");
        GLuint AmbientLightColorID = glGetUniformLocation(shaderID, "ambientLight.Color");
        GLuint AmbientLightIntensityID = glGetUniformLocation(shaderID, "ambientLight.Intensity");
        GLuint DirectionalLightCountID = glGetUniformLocation(shaderID, "directionalLightCount");


        // Transformations Matricies
        glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(mModel.getMatr()[0][0]));
        glUniform3f(CameraPositionID, camera.mPos.x,camera.mPos.y,camera.mPos.z);
        
        // Textures
        if(mModel.diffuseTexture !=0){
            mModel.diffuseTexture->bindToUnit(GL_TEXTURE0);
            GLuint diffuseID = glGetUniformLocation(shaderID, "diffuseTexture");
            glUniform1i(diffuseID, 0);
        }

        if(mModel.normalTexture !=0){
            mModel.normalTexture->bindToUnit(GL_TEXTURE1);
            GLuint normalID = glGetUniformLocation(shaderID, "normalTexture");
            glUniform1i(normalID, 1);
        }

        if(mModel.specularTexture !=0){
            mModel.specularTexture->bindToUnit(GL_TEXTURE2);
            GLuint specularID = glGetUniformLocation(shaderID, "specularTexture");
            glUniform1i(specularID, 2);
        }
        
        // Lights
        // Ambient
        glUniform3f(AmbientLightColorID, 
                lights.ambient.lightColor.r, 
                lights.ambient.lightColor.g, 
                lights.ambient.lightColor.b);
        glUniform1f(AmbientLightIntensityID, lights.ambient.intensity);

        // DirectionalLights
        glUniform1i(DirectionalLightCountID, lights.directionalLights.size());

        for(unsigned int i = 0; i<lights.directionalLights.size(); i++){
            
            DirectionLight light = lights.directionalLights[i];
            std::string lightName = "directionalLight["+std::to_string(i)+"]";
            
            GLuint colorID = glGetUniformLocation(shaderID, (lightName+".Color").c_str());
            glUniform3f(colorID,light.lightColor.r, 
                                light.lightColor.g, 
                                light.lightColor.b);

            GLuint directionID = glGetUniformLocation(shaderID, (lightName+".Direction").c_str());
            glUniform3f(directionID, light.direction.x, 
                                     light.direction.y, 
                                     light.direction.z);
            
            GLuint intensityID = glGetUniformLocation(shaderID, (lightName+".Intensity").c_str());
            glUniform1f(intensityID,  light.intensity);
        }

        mModel.render();
}