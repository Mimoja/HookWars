#include "GameObject.h"
#include "Shader.h"

#include "stdio.h"
	
GameObject::GameObject(char* path){
    printf("Building GameObject from %s\n",path);
    mModel.readFile(path);
}

void GameObject::update(){}

void GameObject::render(GLuint shaderID, glm::mat4 MVP, Camera camera){

      	// Use our shader
		glUseProgram(shaderID);

	    GLuint WorldMatrixID = glGetUniformLocation(shaderID, "WORLD");
        GLuint ModelMatrixID = glGetUniformLocation(shaderID, "MODEL");
    	GLuint CameraPositionID = glGetUniformLocation(shaderID, "CAMERA");

		glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &(mModel.getMatr()[0][0]));
		glUniform3f(CameraPositionID, camera.mPos.x,camera.mPos.y,camera.mPos.z);

		mModel.render();
}
