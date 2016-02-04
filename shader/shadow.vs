#version 330

layout (location = 0) in vec3 Position;

uniform mat4 LIGHT;
uniform mat4 MODEL;

void main()
{
    gl_Position = (LIGHT*MODEL)*vec4(Position, 1.0);
}
