#pragma once
#include "PostProcessing/FrameBuffer.hpp"
#include "Shader.hpp"
#include <memory>
class ScreenQuad
{

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

public:

	
	unsigned quadVAO;
	unsigned quadVBO;
	
	void DrawEffect(unsigned  texture);



	ScreenQuad();
	~ScreenQuad();
};

