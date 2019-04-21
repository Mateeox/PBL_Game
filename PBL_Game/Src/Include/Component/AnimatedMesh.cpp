#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "AnimatedMesh.hpp"

namespace ModelMesh
{

AnimatedMesh::AnimatedMesh(std::vector<Vertex> verticies,
                           std::vector<unsigned int> indicies,
                           std::vector<Texture> textures,
                           std::vector<VertexBoneData> bone_id_weights,
                           Shader & aShaderProgram) : Mesh(verticies, indicies, textures, aShaderProgram),
                            bones_id_weights_for_each_vertex(bone_id_weights)
{
    SetupMesh();
}

void AnimatedMesh::SetupMesh()
{

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO_bones);

    glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertices[0]), &Vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glBufferData(GL_ARRAY_BUFFER, bones_id_weights_for_each_vertex.size() * sizeof(bones_id_weights_for_each_vertex[0]), &bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	//bones
	glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	//indices
	glBindVertexArray(0);

}

void VertexBoneData::addBoneData(unsigned int bone_id, float weight)
{
	for (unsigned int i = 0; i < NUM_BONES_PER_VEREX; i++) 
	{
		if (weights[i] == 0.0) 
		{
			ids[i] = bone_id;
			weights[i] = weight;
			return;
		}
	}
}

ComponentSystem::ComponentType AnimatedMesh::GetComponentType()
{
	return ComponentSystem::AnimatedMesh;
}

}