#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <vector>


class Texture
{
    public:
        Texture();
        bool loadPNG(char* path);
        void bindToUnit(GLenum TextureUnit);
        GLuint ID;
    private:
        void generateMipmap();
        std::vector<unsigned char> image;
        unsigned int width, height;
};

#endif
