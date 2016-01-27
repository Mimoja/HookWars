#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 UVs;
layout (location = 3) in vec3 Tangents;
layout (location = 4) in vec3 Bitangents;

uniform mat4 WORLD;
uniform mat4 MODEL;

out vec2 UV;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitanget;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    Normal = ((WORLD*MODEL)* vec4(Normals, 0.0)).xyz;
    Tangent = ((WORLD*MODEL)* vec4(Tangents, 0.0)).xyz;
    Bitanget = ((WORLD*MODEL)* vec4(Bitangets, 0.0)).xyz;
    UV = UVs;
}
