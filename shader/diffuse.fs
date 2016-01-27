#version 330

in vec2 UV;

out vec4 FragColor;

uniform sampler2D diffuse;

void main(){
        FragColor = vec4(texture(diffuse, UV).rgb, 1.0f);
}
