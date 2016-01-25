#version 330

layout (location = 0) in vec3 Position;
layout (location = 2) in vec2 UVs;

uniform mat4 WORLD;
uniform mat4 MODEL;

out vec2 UV;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    UV = UVs;
}
