#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Model{
    public:
        Model(void  );
        Model (char* file);
        GLuint VAO;
        GLuint VIA;
        GLuint IBO;
		void setPosition(float x,float y,float z);
		void setRotation(float x, float y, float z);
		void setScaling(float x, float y, float z);
        glm::mat4 getMatr();
        void render();
    private:
        int vertexCount;
		glm::vec3 rotation,scaling, translation;
		glm::mat4 sum = glm::mat4(1.0f);
		
};
#endif
