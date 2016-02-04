#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 UVs;
layout (location = 3) in vec3 Tangents;
layout (location = 4) in vec3 Bitangents;

uniform mat4 WORLD;
uniform mat4 MODEL;
uniform mat4 SHADOW;

out vec2 UV;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 worldPos;
out vec4 lightPos;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    Normal = ((WORLD*MODEL)* vec4(Normals, 0.0)).xyz;
    Tangent = ((WORLD*MODEL)* vec4(Tangents, 0.0)).xyz;
    Bitangent = ((WORLD*MODEL)* vec4(Bitangents, 0.0)).xyz;
    worldPos   = ((WORLD*MODEL)*vec4(Position, 1.0)).xyz;  
    lightPos = SHADOW * vec4(Position, 1.0); 
    UV = UVs;
    UV.y = 1-UVs.y;
}
