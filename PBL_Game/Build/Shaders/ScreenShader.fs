#version 330 core
out vec4  color;
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform bool    shake;
uniform float   blur_kernel[9];
uniform vec2    offsets[9];
void main()
{
	color = vec4(0.0f);
	vec3 sample[9];
	if(shake)
        for(int i = 0; i < 9; i++)
            sample[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	if(shake)
    {
        for(int i = 0; i < 9; i++)
            color += vec4(sample[i] * blur_kernel[i], 0.0f);
        color.a = 1.0f;
    } else
    {
        color = texture(screenTexture, TexCoords);
    }
}	

