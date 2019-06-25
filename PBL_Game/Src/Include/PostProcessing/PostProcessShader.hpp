#pragma once
#include "Shader.hpp"
#include <GL/gl3w.h>
class PostProcessShader : public Shader {
public:
	PostProcessShader(const char *vertex_file_path, const char *fragment_file_path);
	void Shake();
	void UpdateTime(float time);
	void Kill();
	void UpdateKill(float time);
private:
	bool shake = false;
	bool kill = false;
	float currTime = 0;
	float currStrength = 0;
	const float shakeTime = 0.5f;
	const float shakeStrength = 0.01f;
	const float killStrength = 0.1f;
	const float maxKillStrength = 100.0f;
	void StopShaking();
	void ResetKill();
	void SetOffset(GLfloat offset);
};