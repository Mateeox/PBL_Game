#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Drawable.hpp"
#include "ConeRenderable.hpp"

namespace ModelMesh
{
using namespace std;

struct Vertex {
    
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture
{
    unsigned int id;
    string type;
    string path;
};

class Mesh : public ConeRenderable
{

  public:
    vector<Vertex> Vertices;
    vector<unsigned int> Indices;
    vector<Texture> Textures;

    Mesh(vector<Vertex> aVertices,
         vector<unsigned int> aIndices,
         vector<Texture> aTextures,
         Shader & aShaderProgram);
    void Draw(Shader* shader,glm::mat4 &  transform) override;
	std::string Serialize() {
		return Drawable::Serialize();
	}
	float* getVertexData() override;
	int getVertexDataSize() override;
	int getVerticesPositionStride() override;
	unsigned int *getIndices() override;
	int getIndicesSize() override;
	
    ComponentSystem::ComponentType GetComponentType() override;
    private:
    void setupMesh();

};

} // namespace ModelMesh