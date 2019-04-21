#pragma once
#include "Component.hpp"
#include <Shader.hpp>
#include <vector>
#include "Model.hpp"
#include "AnimatedMesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

static const unsigned MAX_BONES = 100;
static unsigned m_num_bones = 0;

class AnimatedModel : public ComponentSystem::Component
{

  public:
    AnimatedModel(std::string &path,Shader & aShaderProgram, bool gammaCorrection);
    void Draw(glm::mat4 &  transform);
    Shader & ShaderProgram;


    glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
    aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

  private:

    std::vector<ModelMesh::Texture> textures_loaded;
    std::vector<ModelMesh::AnimatedMesh> meshes;
    std::vector<ModelMesh::BoneMatrix> m_bone_matrices;


    unsigned m_bone_location[MAX_BONES];
    std::map<std::string, uint> m_bone_mapping; 
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string & path);
    void processNode(aiNode *node, const aiScene *scene);
    ModelMesh::AnimatedMesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<ModelMesh::Texture> loadMaterialTextures(const aiScene *scene,aiMaterial *mat, aiTextureType type,
                                         std::string typeName);

	ComponentSystem::ComponentType GetComponentType() override;
	std::string Serialize() {
		return Component::Serialize();
	}
};