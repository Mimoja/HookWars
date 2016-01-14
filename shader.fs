#version 330

in vec4 color;
in vec3 normal;
in vec3 worldPos;
in vec2 UV;

out vec4 FragColor;

struct AmbientLight
{
    vec3 Color;
    float Intensity;
};

struct DirectionalLight
{
    vec3 Color;
    float Intensity;
    vec3 Direction;
    float SpecularIntensity;
    int SpecularPower;
};

struct Attenuation {
    float constant;
    float linear;
    float exponential;
};

struct PointLight {
    vec3 Color;
    vec3 Position;
    float Intensity;
    Attenuation Falloff;
    float SpecularIntensity;
    int SpecularPower;
};

struct SpotLight {
    vec3 Color;
    vec3 Position;
    float Intensity;
    Attenuation Falloff;
    float SpecularIntensity;
    int SpecularPower;
    vec3 Direction;
    float Cutoff;
    float Hardness;
};


uniform AmbientLight ambientLight;

uniform int directionalLightCount;
uniform DirectionalLight directionalLight[10];

uniform int pointLightCount;
uniform SpotLight spotLight[10];

uniform int spotLightCount;
uniform PointLight pointLight[10];

uniform vec3 CAMERA;
uniform mat4 WORLD;

uniform sampler2D DiffuseTextureSampler;
uniform sampler2D NormalTextureSampler;
uniform sampler2D SpecularTextureSampler;



void main(){

    // Diffuse, Normal and Specular Textures can be sampled here.
    vec3 MaterialColor = texture2D( DiffuseTextureSampler, UV ).rgb;
    vec3 MaterialSpecularColor = texture2D( SpecularTextureSampler, UV ).rgb ;
    vec3 MaterialNormalColor = texture2D(NormalTextureSampler, UV).rgb;

    vec3 Normal = normalize(normal);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    
    // Ambient Color
    vec4 AmbientColor = vec4(ambientLight.Color * ambientLight.Intensity, 1.0f);

    // Iterate over the Light sources
    // DirectionalLights
    for(int i=0; i<directionalLightCount; i++){
        DirectionalLight light = directionalLight[i];
        // DiffuseColor
        float DiffuseFactor = dot(Normal, -light.Direction); 
        if (DiffuseFactor > 0) {
            DiffuseColor += vec4(light.Color * light.Intensity *  DiffuseFactor, 1.0f); 

            // Specular Reflection
            vec3 VertexToEye = normalize(CAMERA - worldPos);
            vec3 LightReflect = normalize(reflect(light.Direction, Normal));
            float SpecularFactor = dot(VertexToEye, LightReflect);
            if (SpecularFactor > 0) {
                SpecularFactor = pow(SpecularFactor, light.SpecularPower);
                SpecularColor += vec4(light.Color * SpecularFactor * light.SpecularIntensity , 1.0f);
            }
        }
    }

    // PointLights
    for(int i=0; i<pointLightCount; i++){
        PointLight light = pointLight[i];
        light.Position = (WORLD*vec4(light.Position,1.0f)).xyz;

        vec3 LightDirection = (worldPos - light.Position);
        float Distance = length(LightDirection);
        LightDirection = normalize(LightDirection);

        vec4 PointLightDiffuseColor  = vec4(0, 0, 0, 0);
        vec4 PointLightSpecularColor  = vec4(0, 0, 0, 0);

        // DiffuseColor
        float DiffuseFactor = dot(Normal, -LightDirection);
        if (DiffuseFactor > 0) {
            PointLightDiffuseColor += vec4(light.Color * light.Intensity *  DiffuseFactor, 1.0f);

            // Specular Reflection
            vec3 VertexToEye = normalize(CAMERA - worldPos);
            vec3 LightReflect = normalize(reflect(LightDirection, Normal));
            float SpecularFactor = dot(VertexToEye, LightReflect);
            if (SpecularFactor > 0) {
                SpecularFactor = pow(SpecularFactor, light.SpecularPower);
                PointLightSpecularColor = vec4(light.Color * SpecularFactor * light.SpecularIntensity , 1.0f);
            }
        }

        float falloff = light.Falloff.constant + light.Falloff.linear * Distance + light.Falloff.exponential * Distance * Distance;
        DiffuseColor += PointLightDiffuseColor / falloff;
        SpecularColor += PointLightSpecularColor / falloff;
    }

    // SpotLights
    for(int i=0; i<spotLightCount; i++){
        SpotLight light = spotLight[i];
        light.Position = (WORLD*vec4(light.Position,1.0f)).xyz;

        vec3 LightDirection = (worldPos - light.Position);
        vec3 LightToPixel = normalize(LightDirection);                             
        float SpotFactor = dot(LightToPixel, light.Direction);   
        if (SpotFactor  > light.Cutoff) { 
            float Distance = length(LightDirection);

            vec4 SpotLightDiffuseColor  = vec4(0, 0, 0, 0);
            vec4 SpotLightSpecularColor  = vec4(0, 0, 0, 0);

            // DiffuseColor
            float DiffuseFactor = dot(Normal, -LightDirection);
            if (DiffuseFactor > 0) {
                SpotLightDiffuseColor += vec4(light.Color * light.Intensity *  DiffuseFactor, 1.0f);

                // Specular Reflection
                vec3 VertexToEye = normalize(CAMERA - worldPos);
                vec3 LightReflect = normalize(reflect(LightToPixel, Normal));
                float SpecularFactor = dot(VertexToEye, LightReflect);
                if (SpecularFactor > 0) {
                    SpecularFactor = pow(SpecularFactor, light.SpecularPower);
                    SpotLightSpecularColor = vec4(light.Color * SpecularFactor * light.SpecularIntensity , 1.0f);
                }
            }

            float falloff = light.Falloff.constant + light.Falloff.linear * Distance + light.Falloff.exponential * Distance * Distance;
            float blurFactor = SpotFactor-light.Cutoff;
            if(blurFactor<light.Hardness){
                SpotLightDiffuseColor *= blurFactor;
            }
            DiffuseColor += SpotLightDiffuseColor  / falloff;
            SpecularColor += SpotLightSpecularColor  / falloff;
        }
    }

    FragColor = color * (AmbientColor + DiffuseColor +  SpecularColor);
}
