#pragma once
#include "Component.hpp"
#include <Shader.hpp>
#include <vector>
#include "Mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public ComponentSystem::Component
{

  public:
    Model(std::string &path,Shader & aShaderProgram, bool gammaCorrection);
    void Draw(glm::mat4 &  transform);
    Shader & ShaderProgram;

    unsigned NumBones() const
    {
        return m_NumBones;
    }

  private:

    std::vector<ModelMesh::Texture> textures_loaded;
    std::vector<ModelMesh::Mesh> meshes;
    std::map<std::string,unsigned> m_BoneMapping;
    std::vector<ModelMesh::BoneInfo> m_BoneInfo;
    std::vector<ModelMesh::MeshEntry> m_Entries;

    unsigned m_NumBones;
    std::string directory;
    bool gammaCorrection;

    void loadModel(std::string & path);
    void processNode(aiNode *node, const aiScene *scene);
    ModelMesh::Mesh processMesh(unsigned aMeshID,aiMesh *mesh, const aiScene *scene);
    std::vector<ModelMesh::Texture> loadMaterialTextures(const aiScene *scene,aiMaterial *mat, aiTextureType type,
                                         std::string typeName);

	ComponentSystem::ComponentType GetComponentType() override;
	std::string Serialize() {
		return Component::Serialize();
	}
};