#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 viewSpace;

uniform sampler2D texture_diffuse1;

const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor = vec3(0.2, 0.2, 0.2);

float dist = 0;
float fogFactor = 0;
 
const vec3 fogColor = vec3(0.5, 0.5,0.5);
uniform float FogDensity = 0.15;

vec3 finalColor = vec3(0, 0, 0);

uniform float viewSpaceZOffset = 5.0f;

void main()
{    
    dist = abs(viewSpace.z-viewSpaceZOffset);

    fogFactor = (80 - dist)/(80 - 20);
   fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
    fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
   fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
   finalColor = (fogColor * fogFactor);

    //finalColor = finalColor * texture(texture_diffuse1, TexCoords);
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(finalColor,1);
}