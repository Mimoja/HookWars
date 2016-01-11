#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

class Shader{
    public:
        Shader(char * vertex_file_path, char * fragment_file_path);
        GLuint getShaderID();
    private:
        GLuint mID;
};

#endif
