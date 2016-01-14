#include "ShadowMap.h"
#include <GLFW/glfw3.h>

#include "stdio.h"
#include <string>
#include <string.h>

void ShadowMap::create(int width, int height) {

    glGenFramebuffers(1, &frameBuffer);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0
        , GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum res = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (res != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer creation error: 0x%x \n", res);
        exit(-1);
    }
}

void ShadowMap::destroy() {
    if (frameBuffer != 0) {
        glDeleteFramebuffers(1, &frameBuffer);
    }

    if (shadowMap != 0) {
        glDeleteTextures(1, &shadowMap);
    }
}

void ShadowMap::Bind() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
}

void ShadowMap::Bind(GLenum texUnit) {
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
}


