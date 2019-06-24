#include "PostProcessShader.hpp"

PostProcessShader::PostProcessShader(const char *vertex_file_path, const char *fragment_file_path) : Shader(vertex_file_path, fragment_file_path)
{
	GLfloat blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->shaderProgramID, "blur_kernel"), 9, blur_kernel);
	this->setFloat("time", shakeTime);
}

void PostProcessShader::Shake()
{
	if (!shake) {
		shake = true;
		this->setBool("shake", shake);
	}
}

void PostProcessShader::StopShaking()
{
	if (shake)
	{
		shake = false;
		this->setBool = shake;
	}
}
