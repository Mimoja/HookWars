#version 330                                                                        
                                                                                    
in vec2 UV;                                                                
uniform sampler2D ShadowMapSampler;                                                       
                                                                                    
out vec4 FragColor;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    float Depth = texture(ShadowMapSampler, UV).x; 
                              
    Depth = 1.0 - (1.0 - Depth) * 25.0;
                                             
    FragColor = vec4(Depth);                                                        
}
