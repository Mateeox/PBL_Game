#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 viewSpace;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
    viewSpace = gl_Position; 
	Normal = mat3(transpose(inverse(transform))) * aNormal;  	
	FragPos = vec3(transform * vec4(aPos, 1.0));
}