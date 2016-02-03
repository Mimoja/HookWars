/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HealthBar.cpp
 * Author: maximilian
 * 
 * Created on 3. Februar 2016, 11:11
 */

#include "HealthBar.h"
#include "util.h"

extern GLuint healthBarShaderID;

float health = 0.3f;

HealthBar::HealthBar(Player * o) : GameObject(HEALTHBAR_MODEL) {
    mModel.scaling = glm::vec3(HEALTHBAR_LENGTH, HEALTHBAR_SCALING, 1.0f);
    origin = o;
}

void HealthBar::update() {
    if (origin != 0) {
        mModel.position = origin->mModel.position;
        mModel.position.y += HEALTHBAR_HEIGHT;
    } 
}

void HealthBar::render(GLuint shaderID, glm::mat4 MVP, Camera camera, Lights lights) {

    glUseProgram(healthBarShaderID);

    GLint healthID = glGetUniformLocation(healthBarShaderID, "HEALTH");
    if (origin != 0) {
        if (healthID != -1) glUniform1f(healthID, origin->health);
    }
    GameObject::render(healthBarShaderID, MVP, camera, *(new Lights()));
}
