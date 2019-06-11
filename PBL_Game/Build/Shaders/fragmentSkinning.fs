#version 330

in vec2 TexCoord0;
in vec3 Normal0;                                                                   
in vec3 WorldPos0;                                                                 

struct VSOutput
{
    vec2 TexCoord;
    vec3 Normal;                                                                   
    vec3 WorldPos;                                                                 
};
                                                                                                                                                                                                                                                    
uniform sampler2D gColorMap;                                                                
                                                                                                                                                                                                                                                      
out vec4 FragColor;

struct PointLight
{
    vec3 lightPos;
    float Kambient;
    float kSpecular; 
    float Kdiffuse;
}

/*
 uniform   vec3 lightPos;
 uniform   float Kambient;
 uniform   float kSpecular; 
 uniform   float Kdiffuse;
*/

uniform vec3 lightPos;
uniform vec3 viewPos; //camera position
uniform bool blinn;  //use Phon-blinn or Phong
                                                                
void main()
{                                    
    VSOutput In;
    In.TexCoord = TexCoord0;
    In.Normal   = normalize(Normal0);
    In.WorldPos = WorldPos0;


  vec3 color = texture(gColorMap,In.TexCoord).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, In.Normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, In.Normal);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(In.Normal, halfwayDir), 0.0), 32.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);

}
