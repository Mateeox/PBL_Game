#pragma once
#include "Shader.hpp"
#include <GL/gl3w.h>
class PostProcessShader : public Shader {
public:
	PostProcessShader(const char *vertex_file_path, const char *fragment_file_path);
	void Shake();
	void StopShaking();
private:
	bool shake = false;
	const float shakeTime = 0.5f;
};