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
    float SpecularIntensity;
    int SpecularPower;
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

    // Iterate over the Light sources
    for(int i=0; i<directionalLightCount; i++){
        // DiffuseColor
        float DiffuseFactor = dot(Normal, directionalLight[i].Direction); 
        if (DiffuseFactor > 0) {                                                        
            DiffuseColor += vec4(directionalLight[i].Color * directionalLight[i].Intensity *  DiffuseFactor, 1.0f); 

            // Specular Reflection
            vec3 VertexToEye = normalize(CAMERA - worldPos);                     
            vec3 LightReflect = normalize(reflect(directionalLight[i].Direction, Normal));
            float SpecularFactor = dot(VertexToEye, LightReflect);                      
            if (SpecularFactor > 0) {                                                   
                SpecularFactor = pow(SpecularFactor, directionalLight[i].SpecularPower);
                SpecularColor += vec4(ambientLight.Color * directionalLight[i].SpecularIntensity * SpecularFactor, 1.0f);            }                                                                           
        }
    }

    FragColor = color * (AmbientColor + DiffuseColor +  SpecularColor);                                                                                                                                       
}
