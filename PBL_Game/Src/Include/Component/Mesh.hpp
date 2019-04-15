#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Drawable.hpp"

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

class Mesh : public Drawable
{

  public:
    vector<Vertex> Vertices;
    vector<unsigned int> Indices;
    vector<Texture> Textures;

    Mesh(vector<Vertex> aVertices,
         vector<unsigned int> aIndices,
         vector<Texture> aTextures,
         Shader & aShaderProgram);
    void Draw(glm::mat4 &  transform) override;

    ComponentSystem::ComponentType GetComponentType() override;
    private:
    void setupMesh();

	std::string Serialize() {
		return Drawable::Serialize();
	}
};

} // namespace ModelMesh