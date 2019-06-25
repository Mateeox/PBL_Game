#include "PostProcessShader.hpp"

PostProcessShader::PostProcessShader(const char *vertex_file_path, const char *fragment_file_path) : Shader(vertex_file_path, fragment_file_path)
{
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right    
	};
	
	GLfloat blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	this->use();
	glUniform2fv(glGetUniformLocation(this->shaderProgramID, "offsets"), 9, (GLfloat*)offsets);
	glUniform1fv(glGetUniformLocation(this->shaderProgramID, "blur_kernel"), 9, blur_kernel);
	this->setFloat("time", shakeTime);
	this->setBool("shake", shake);
}

void PostProcessShader::Shake()
{
	if (!shake) {
		shake = true;
		currTime = shakeTime;
		this->use();
		this->setFloat("time", currTime);
		this->setBool("shake", shake);
	}
}

void PostProcessShader::StopShaking()
{
	if (shake)
	{
		shake = false;
		this->use();
		this->setBool("shake", shake);
	}
}

void PostProcessShader::UpdateTime(float time)
{
	currTime -= time;
	if (currTime <= 0) {
		StopShaking();
	} else {
		this->use();
		setFloat("time", currTime);
	}
}
