#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;



//ColorValue
uniform vec3 ColorFade;

uniform bool fadeToColor;
uniform bool fadeFromTransparent;

uniform float fadeFromTransparentValue;
uniform float fadeToColorValue;


void main()
{
    vec4 texColor = texture(texture1, TexCoord);

    vec4 fadeToColorVec4;
    vec4 fadeToTransparentVec4;

    if(texColor.a < 0.1)
        discard;

        fadeToColorVec4 =  (vec4(ColorFade * fadeToColorValue , 1.0));
        fadeToTransparentVec4 = texture(texture1, TexCoord) * fadeFromTransparentValue;

        if(fadeToColor && !fadeFromTransparent)
        {
          FragColor = fadeToColorVec4;
        }
        else if(fadeFromTransparent && !fadeToColor)
        {
             FragColor = fadeToTransparentVec4;
        }
        else if (fadeToColor && fadeFromTransparent)
        {
            FragColor = mix(fadeToColorVec4,fadeToTransparentVec4,0.75) * 2;
        }
        else
        {
            FragColor = texture(texture1, TexCoord);
        }


}