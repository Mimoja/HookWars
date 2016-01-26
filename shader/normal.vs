#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 UVs;

uniform mat4 WORLD;
uniform mat4 MODEL;

out vec2 UV;
out vec3 Normal;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    Normal = ((WORLD*MODEL)* vec4(Normals, 0.0)).xyz;
    UV = UVs;
}
