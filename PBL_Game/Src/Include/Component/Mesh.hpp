#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Drawable.hpp"
#include <map>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#include "BoneUtils.hpp"

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



  struct BoneInfo
    {
        Matrix4f BoneOffset;
        Matrix4f  FinalTransformation;        

        BoneInfo()
        {
            BoneOffset.SetZero();
            FinalTransformation.SetZero();        
        }
    };

  struct VertexBoneData
    {        
        unsigned IDs[4];
        float Weights[4];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(unsigned BoneID, float Weight);
    };


class Mesh : public Drawable
{

  public:
    vector<Vertex> Vertices;
    vector<unsigned int> Indices;
    vector<Texture> Textures;
    vector<VertexBoneData> Bones;
   

    Mesh(vector<Vertex> aVertices,
         vector<unsigned int> aIndices,
         vector<Texture> aTextures,
         vector<VertexBoneData> aBones,
         Shader & aShaderProgram);
    void Draw(glm::mat4 &  transform) override;
	std::string Serialize() {
		return Drawable::Serialize();
	}
	
    ComponentSystem::ComponentType GetComponentType() override;
    private:
    void setupMesh();

};

} // namespace ModelMesh