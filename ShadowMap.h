#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <GL/glew.h>

class ShadowMap {
public:
    void create(int width, int height);
    void destroy();
    void bind();
    void bind(GLenum texUnit);

private:
    GLuint frameBuffer;
    GLuint shadowMap;
    GLuint shaderID;

};

#endif /* SHADOWMAP_H */

