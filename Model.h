#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Model{
    public:
        Model(){}
        void readFile(char* file);
		void setPosition(float x,float y,float z);
		void setRotation(float x, float y, float z);
		void setScaling(float x, float y, float z);
        glm::mat4 getMatr();
        void render();
    private:
        GLuint elementBuffer;
        GLuint vertexBuffer;
        GLuint normalBuffer;
        GLuint uvBuffer;
        GLuint VertexArrayID;
        int indicesCount;
		glm::vec3 rotation,scaling, translation;
		glm::mat4 sum = glm::mat4(1.0f);
		
};
#endif
