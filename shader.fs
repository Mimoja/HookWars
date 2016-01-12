#version 330

in vec4 color;
in vec3 normal;  
in vec3 worldPos;

out vec4 FragColor;

struct DirectionalLight                                                             
{                                                                                   
    vec3 Color;                                                                     
    float AmbientIntensity;                                                         
    float DiffuseIntensity;                                                         
    vec3 Direction;  
    float SpecularIntensity;       
    float SpecularPower;                                                        
};       

uniform DirectionalLight gDirectionalLight;
uniform vec3 CAMERA;  

void main()                                                                         
{                                                                                   
    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0f);
    vec3 LightDirection = -gDirectionalLight.Direction;                             
    vec3 Normal = normalize(normal);                                               
                                                                                    
    float DiffuseFactor = dot(Normal, LightDirection);                              
                                                                                    
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                          
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                          
                                                                                    
    if (DiffuseFactor > 0) {                                                        
        DiffuseColor = vec4(gDirectionalLight.Color * gDirectionalLight.DiffuseIntensity * DiffuseFactor, 1.0f);
                                                                                    
        vec3 VertexToEye = normalize(CAMERA - worldPos);                     
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);                      
        if (SpecularFactor > 0) {                                                   
            SpecularFactor = pow(SpecularFactor, gDirectionalLight.SpecularPower);
            SpecularColor = vec4(gDirectionalLight.Color * gDirectionalLight.SpecularIntensity * SpecularFactor, 1.0f);
        }                                                                           
    }                                                                               
                                                                                    
    FragColor = color * (AmbientColor + DiffuseColor + SpecularColor);                      
}
