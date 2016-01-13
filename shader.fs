#version 330

in vec4 color;
in vec3 normal;  
in vec3 worldPos;

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
    int SpecularIntensity;
};

uniform AmbientLight ambientLight;

uniform int directionalLightCount;
uniform DirectionalLight directionalLight[10];

uniform vec3 CAMERA;

void main()                                                                         
{    
    vec3 Normal = normalize(normal);                                               

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    // Ambient Color
    vec4 AmbientColor = vec4(ambientLight.Color * ambientLight.Intensity, 1.0f);

    // DiffuseColor
    float DiffuseFactor = dot(Normal, directionalLight[0].Direction); 
    if (DiffuseFactor > 0) {                                                        
        DiffuseColor = vec4(directionalLight[0].Color * directionalLight[0].Intensity *  DiffuseFactor, 1.0f); 

        // Specular Reflection
        vec3 VertexToEye = normalize(CAMERA - worldPos);                     
        vec3 LightReflect = normalize(reflect(directionalLight[0].Direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);                      
        if (SpecularFactor > 0) {                                                   
            SpecularFactor = pow(SpecularFactor, directionalLight[0].SpecularIntensity);
            SpecularColor = vec4(ambientLight.Color * directionalLight[0].SpecularIntensity * SpecularFactor, 1.0f);
        }                                                                           
    }

    FragColor = color * (AmbientColor + DiffuseColor + SpecularColor);                                                                                                                                       
}
