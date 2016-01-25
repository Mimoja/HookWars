#version 330

in vec2 UV;

out vec4 FragColor;

uniform sampler2D diffuse;
uniform vec3 color;

void main(){
    if(color == vec3(0.0f)){
        FragColor = vec4(texture(diffuse, UV).rgb, 1.0f);
    }else{
        FragColor =  vec4(color, 1.0f);
    }
}
