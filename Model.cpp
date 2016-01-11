#include "Model.h"
#include "util.h"

Model::Model(void){}

Model::Model(char* path){
    //loadModelFromFile(path,);
}

void Model::setPosition(float x,float y,float z){
	translation=glm::vec3(x,y,z);
}
void Model::setRotation(float x, float y, float z){
	rotation=glm::vec3(x,y,z);
}
void Model::setScaling(float x, float y, float z){
	scaling=glm::vec3(x,y,z);
}
glm::mat4 Model::getMatr(){
	sum=glm::mat4(1.0f);
	sum=glm::scale(sum,scaling);
	sum=glm::rotate(sum,rotation[0],glm::vec3(1,0,0));
    sum=glm::rotate(sum,rotation[1],glm::vec3(0,1,0));
    sum=glm::rotate(sum,rotation[2],glm::vec3(0,0,1));
	
	sum=glm::translate(sum,translation);
	return sum;
}

void Model::render(){

    // VAA 0 Verticies
    glEnableVertexAttribArray(0);
    // VAA 0 Normals
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
}
