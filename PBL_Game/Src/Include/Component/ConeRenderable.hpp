#pragma once
#include "Drawable.hpp"
#include "Texture.hpp"
#include <vector>
#include <map>
#include <array>

class ConeRenderable : public Drawable
{
	std::array<glm::vec4, 8> extrema;
	void calculateExtrema(float* vertexBufferData, int size);
	int stride;
	public:
		ConeRenderable(Shader &aShaderProgram, float *g_vertex_buffer_data, int size, int stride);
		std::array<glm::vec4, 8> getExtrema() { return extrema; }
		virtual float* getVertexData() = 0;
		virtual int getVertexDataSize() = 0;
		virtual int getVerticesPositionStride() = 0;
		virtual unsigned int *getIndices() = 0;
		virtual int getIndicesSize() = 0;
	};