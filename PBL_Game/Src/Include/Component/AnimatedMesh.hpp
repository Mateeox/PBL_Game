#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#define NUM_BONES_PER_VEREX 4


namespace ModelMesh
{
struct BoneMatrix
{
    aiMatrix4x4 offset_matrix;
    aiMatrix4x4 final_world_transform;
};

struct VertexBoneData
{
    unsigned int ids[NUM_BONES_PER_VEREX]; // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
    float weights[NUM_BONES_PER_VEREX];

    VertexBoneData()
    {
        memset(ids, 0, sizeof(ids)); // init all values in array = 0
        memset(weights, 0, sizeof(weights));
    }

    void addBoneData(unsigned int bone_id, float weight);
};

class AnimatedMesh : public Mesh
{

  public:
    AnimatedMesh(std::vector<Vertex> vertic,
                 std::vector<unsigned int> ind,
                 std::vector<Texture> textur,
                 std::vector<VertexBoneData> bone_id_weights,
                 Shader &aShaderProgram);

    vector<VertexBoneData> bones_id_weights_for_each_vertex;
    unsigned int VBO_bones;

    void SetupMesh();
    private:
    void addBoneData(unsigned int bone_id, float weight);
};
}