#include "Texture.h"

#include "lodepng.h"

#include "stdio.h"

Texture::Texture(){
    glGenTextures(1, &ID);
}

void Texture::bindToUnit(GLenum textureUnir){
    glActiveTexture(textureUnir);
    glBindTexture(GL_TEXTURE_2D, ID);
}

bool Texture::loadPNG(const char* path){
    printf("Loading Texture from %s\n",path);
    unsigned int res = lodepng::decode(image, width, height, path);

    if(res != 0)
    {
        printf("error %d : %s\n",res,lodepng_error_text(res));
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    generateMipmap();

    return true;
}

void Texture::generateMipmap(){

    glBindTexture(GL_TEXTURE_2D, ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

}

