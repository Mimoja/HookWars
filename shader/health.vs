#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 UVs;
layout (location = 3) in vec3 Tangents;
layout (location = 4) in vec3 Bitangents;

uniform mat4 WORLD;
uniform mat4 MODEL;


uniform float HEALTH;

void main()
{
    vec3 pos = Position;
    pos.x *= HEALTH;
    pos.x -= 1.0f;
    gl_Position = (WORLD*MODEL)*vec4(pos, 1.0);
}
