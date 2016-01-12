#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec3 UVs;

uniform mat4 WORLD;
uniform mat4 MODEL;

out vec4 color;
out vec3 normal;
out vec3 worldPos;

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    color = vec4(Normal, 1.0f);
    normal = ( vec4(Normal, 0.0)).xyz;
    worldPos   = (WORLD * vec4(Position, 1.0)).xyz;      
}
