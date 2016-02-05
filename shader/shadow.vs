#version 330 core

layout(location = 0) in vec3 Position;

uniform mat4 depthMVP;
uniform mat4 MODEL;

void main(){
    gl_Position =  (depthMVP*MODEL) * vec4(Position, 1.0f);
}

