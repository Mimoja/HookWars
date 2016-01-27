#version 330

in vec2 UV;
in vec3 Normal;
in vec3 Tangent;
in vec3 Bitanget;

out vec4 FragColor;

uniform sampler2D normalSampler;
uniform int useNormalTexture;

void main(){
        vec3 normal = normalize(Normal);
        vec3 tangent = normalize(Tangent);
        vec3 bitangent = normalize(Bitangent);

        vec3 textureNormal = texture(normalSampler, UV).xyz;
        textureNormal = 2.0 * textureNormal - vec3(1.0, 1.0, 1.0);
        vec3 finalNormal;
        mat3 TBN = mat3(Tangent, Bitangent, Normal);
        finalNormal = TBN * textureNormal;
        finalNormal = normalize(finalNormal);

        FragColor = vec4(finalNormal,1.0f);
}