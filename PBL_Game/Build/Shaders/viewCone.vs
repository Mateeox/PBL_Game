#version 330 core
layout (location = 0) in vec3 aPos;

out vec4 outColor;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * vec4(aPos, 1.0);
  outColor = vec4(0.0, 1.0, 0.0, 0.3);
};

