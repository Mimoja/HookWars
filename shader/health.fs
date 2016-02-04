#version 330

out vec4 FragColor;

uniform float HEALTH;

void main(){

        FragColor = vec4(1.0f-HEALTH,HEALTH,0.0f,1.0f);
}
