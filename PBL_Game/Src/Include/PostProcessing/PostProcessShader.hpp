#pragma once
#include "Shader.hpp"
#include <GL/gl3w.h>
class PostProcessShader : public Shader {
public:
	PostProcessShader(const char *vertex_file_path, const char *fragment_file_path);
	void Shake();
	void UpdateTime(float time);
private:
	bool shake = false;
	float currTime = 0;
	const float shakeTime = 0.5f;
	void StopShaking();
};