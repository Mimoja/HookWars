#version 330

in vec4 color;
in vec3 normal;  
in vec3 worldPos;

out vec4 FragColor;

uniform vec3 CAMERA;  

void main()                                                                         
{                                                                                                                         
    FragColor = color;                   
}
