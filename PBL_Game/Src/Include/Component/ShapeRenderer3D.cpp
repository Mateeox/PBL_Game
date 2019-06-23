#include "ShapeRenderer3D.hpp"
#include "GameObject.hpp"
// Include GLEW
#include <GL/gl3w.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>

#include <stdio.h>
#include <stdlib.h>
ShapeRenderer3D::ShapeRenderer3D(
	GLfloat *g_ver,
	unsigned int *aindices,
	int size,
	int aindices_size,
	Shader &aShaderProgram,
	Texture *atexture,
	std::string textureName) : ConeRenderable(aShaderProgram, g_ver, size, getVerticesPositionStride()),
							   g_vertex_buffer_data(g_ver),
							   g_vertex_buffer_data_size(size),
							   indices_size(aindices_size),
							   indices(aindices),
							   textureDisplayed(textureName)
{
	textures[textureName] = atexture;
	what_Draw_use = 1;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind vertex Arrary object
	glBindVertexArray(VAO);
	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);

	//position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	//color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

ShapeRenderer3D::ShapeRenderer3D(
	GLfloat *g_ver, int size,
	Shader &aShaderProgram,
	Texture *atexture,
	std::string textureName) : ConeRenderable(aShaderProgram, g_ver, size, getVerticesPositionStride()),
							   g_vertex_buffer_data(g_ver),
							   g_vertex_buffer_data_size(size),
							   textureDisplayed(textureName)
{
	textures[textureName] = atexture;

	what_Draw_use = 2;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vertex Arrary object
	glBindVertexArray(VAO);
	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, g_vertex_buffer_data, GL_STATIC_DRAW);

	//position attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	//color attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void ShapeRenderer3D::Draw(Shader * shader,glm::mat4 &transform)
{
    if(shader == nullptr)
	{
	defaultShader.use();
	unsigned int transformLoc = glGetUniformLocation(defaultShader.shaderProgramID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	}
	else
	{
	shader->use();
	unsigned int transformLoc = glGetUniformLocation(shader->shaderProgramID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		
	}
	
	
	if (textures.size() != 0)
	{ 
		textures[textureDisplayed]->Bind();
	}
	if (what_Draw_use == 1)
	{
		Draw_Elements();
	}
	else if (what_Draw_use == 2)
	{
		Draw_Arrays();
	}

	glActiveTexture(GL_TEXTURE0);
}

void ShapeRenderer3D::Draw_Elements()
{

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0); // 3 indices starting at 0 -> 1 triangle
}

ComponentSystem::ComponentType ShapeRenderer3D::GetComponentType()
{
	return ComponentSystem::ComponentType::ShapeRenderer3D;
}

void ShapeRenderer3D::Draw_Arrays()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data_size); // 3 indices starting at 0 -> 1 triangle
}

void ShapeRenderer3D::calculateExtrema(float* vertexBufferData, int size)
{
	
	glm::mat2x3 extremaTmp{
		vertexBufferData[0], vertexBufferData[0], vertexBufferData[0],
		vertexBufferData[0], vertexBufferData[0], vertexBufferData[0]
	};
	glm::vec3 maxPoint{
		vertexBufferData[0], vertexBufferData[1], vertexBufferData[2]
	};
	glm::vec3 minPoint{
		vertexBufferData[0], vertexBufferData[1], vertexBufferData[2]
	};
	//3 for position, 3 for color and 2 for textures, hence 8
	for (unsigned int i = 0; i < size; i += 8)
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

void ShapeRenderer3D::SwitchTexture(std::string textureName)
{
	textureDisplayed = textureName;
}

ShapeRenderer3D *ShapeRenderer3D::GetCopy()
{
	ShapeRenderer3D *newCopy = new ShapeRenderer3D(g_vertex_buffer_data, indices, g_vertex_buffer_data_size, indices_size, defaultShader, textures[0], textureDisplayed);

	return newCopy;
}

void ShapeRenderer3D::AsignTexture(Texture *aTexture, std::string aTextureName)
{
	textures[aTextureName] = aTexture;
}

std::array<glm::vec4, 8> ShapeRenderer3D::getExtrema()
{
	return extrema;
}

float* ShapeRenderer3D::getVertexData()
{
	return g_vertex_buffer_data;
}

int ShapeRenderer3D::getVertexDataSize()
{
	return g_vertex_buffer_data_size / sizeof(decltype(g_vertex_buffer_data_size));
}

int ShapeRenderer3D::getVerticesPositionStride()
{
	return 8;
}

unsigned int * ShapeRenderer3D::getIndices()
{
	return indices;
}

int ShapeRenderer3D::getIndicesSize()
{
	return indices_size / sizeof(decltype(indices_size));
}

ShapeRenderer3D::~ShapeRenderer3D()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
