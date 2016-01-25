#version 330

in vec2 UV;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D normalSampler;
uniform int useNormalTexture;

void main(){

        vec3 normal = texture(normalSampler, UV).rgb;
        normal = normalize(normal * 2.0 - 1.0);  

        FragColor = vec4(Normal,1.0f)-vec4(normal,1.0f);
}