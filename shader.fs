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
};

uniform AmbientLight ambientLight;
uniform DirectionalLight directionalLight;
uniform vec3 CAMERA;

void main()                                                                         
{    
    vec3 Normal = normalize(normal);                                               

    vec3 LightDirection = vec3(3,0.1,1);
    float DiffuseIntensity = 1.0f;



    vec4 DiffuseColor  = vec4(0, 0, 0, 0);

    
   
    vec4 AmbientColor = vec4(ambientLight.Color * ambientLight.Intensity, 1.0f);

    float DiffuseFactor = dot(Normal, LightDirection); 
    if (DiffuseFactor > 0) {                                                        
        DiffuseColor = vec4(vec3(1.0f,1.0f,1.0f) * DiffuseIntensity * DiffuseFactor, 1.0f); 
        
    }  
    /*                                                                       
    
    vec3 LightDirection = -gDirectionalLight.Direction;                             
                              
                                                                                                                       
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                          

    DiffuseFactor > 0
                                                                        
        vec3 VertexToEye = normalize(CAMERA - worldPos);                     
        vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);                      
        if (SpecularFactor > 0) {                                                   
            SpecularFactor = pow(SpecularFactor, gDirectionalLight.SpecularPower);
            SpecularColor = vec4(gDirectionalLight.Color * gDirectionalLight.SpecularIntensity * SpecularFactor, 1.0f);
        }                                                                           
    }                                                                               
                                                                                    
    FragColor = color * (AmbientColor + DiffuseColor + SpecularColor);        */                                                                                                                               
    FragColor = color * (AmbientColor + DiffuseColor);                   
}
