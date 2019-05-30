#include "ConeRenderable.hpp"

void ConeRenderable::calculateExtrema(float* vertexBufferData, int size)
{
	glm::vec3 maxPoint{
		vertexBufferData[0], vertexBufferData[1], vertexBufferData[2]
	};
	glm::vec3 minPoint{
		vertexBufferData[0], vertexBufferData[1], vertexBufferData[2]
	};
	//3 for position, 3 for color and 2 for textures, hence 8
	for (unsigned int i = 0; i < size; i += stride)
	{
		if (vertexBufferData[i] <= minPoint.x && vertexBufferData[i + 1] <= minPoint.y && vertexBufferData[i + 2] <= minPoint.z)
		{
			minPoint.x = vertexBufferData[i];
			minPoint.y = vertexBufferData[i + 1];
			minPoint.z = vertexBufferData[i + 2];
		}
		else if (vertexBufferData[i] >= minPoint.x && vertexBufferData[i + 1] >= minPoint.y && vertexBufferData[i + 2] >= minPoint.z)
		{
			maxPoint.x = vertexBufferData[i];
			maxPoint.y = vertexBufferData[i + 1];
			maxPoint.z = vertexBufferData[i + 2];
		}
	}

	extrema = {
		glm::vec4(minPoint.x, minPoint.y, minPoint.z, 1),
		glm::vec4(minPoint.x, minPoint.y, maxPoint.z, 1),
		glm::vec4(minPoint.x, maxPoint.y, minPoint.z, 1),
		glm::vec4(minPoint.x, maxPoint.y, maxPoint.z, 1),
		glm::vec4(maxPoint.x, minPoint.y, minPoint.z, 1),
		glm::vec4(maxPoint.x, minPoint.y, maxPoint.z, 1),
		glm::vec4(maxPoint.x, maxPoint.y, minPoint.z, 1),
		glm::vec4(maxPoint.x, maxPoint.y, maxPoint.z, 1)
	};
}

ConeRenderable::ConeRenderable(Shader & aShaderProgram, float *g_vertex_buffer_data, int size, int stride) 
	: Drawable(aShaderProgram), stride(stride)
{
	calculateExtrema(g_vertex_buffer_data, size / sizeof(decltype(size)));
}
