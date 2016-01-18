#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UVs;

uniform mat4 WORLD;
uniform mat4 MODEL;
uniform mat4 LIGHT; 

out vec4 color;
out vec3 normal;
out vec3 worldPos;
out vec4 lightPos; 
out vec2 UV;

uniform vec3 teamColor;
uniform int useTeamColor; 

void main()
{
    gl_Position = (WORLD*MODEL)*vec4(Position, 1.0);
    normal = ((WORLD*MODEL)* vec4(Normal, 0.0)).xyz;
    if(vec4(teamColor,0.0f)!=vec4(0.0f)){
        color = vec4(teamColor, 1.0f);
    }else{
        color = vec4(0.8f,0.8f,0.8f,1.0f);
    }
    worldPos   = ((WORLD*MODEL)*vec4(Position, 1.0)).xyz;  
    lightPos = LIGHT * vec4(Position, 1.0); 
    UV = UVs;
}
