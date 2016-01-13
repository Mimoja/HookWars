#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UVs;

uniform mat4 WORLD;
uniform mat4 MODEL;

out vec4 color;
out vec3 normal;
out vec3 worldPos;
out vec2 UV;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    normal = ((WORLD*MODEL)* vec4(Normal, 0.0)).xyz;
    color = vec4(0.8f,0.8f,0.8f,1.0f);
    worldPos   = (MODEL* vec4(Position, 1.0)).xyz;  
    UV = UVs;
}
