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
    } else if (healthID != -1) glUniform1f(healthID, 1.0f);

    GameObject::render(healthBarShaderID, MVP, camera, *(new Lights()));
}
