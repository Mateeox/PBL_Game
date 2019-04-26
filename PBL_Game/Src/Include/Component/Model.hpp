#pragma once

#include "Component.hpp"
#include <Shader.hpp>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "BoneUtils.hpp"
#include "Texture.hpp"
#include <map>


#ifdef WIN32
#define SNPRINTF _snprintf_s
#define VSNPRINTF vsnprintf_s
#define RANDOM rand
#define SRANDOM srand((unsigned)time(NULL))
#else
#define SNPRINTF snprintf
#define VSNPRINTF vsnprintf
#define RANDOM random
#define SRANDOM srandom(getpid())
#endif

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

#define ZERO_MEM(a) memset(a, 0, sizeof(a))

#define POSITION_LOCATION 0
#define TEX_COORD_LOCATION 1
#define NORMAL_LOCATION 2
#define BONE_ID_LOCATION 3
#define BONE_WEIGHT_LOCATION 4

#define NUM_BONES_PER_VEREX 4

#define INVALID_MATERIAL 0xFFFFFFFF

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof(a[0]))
static const unsigned MAX_BONES = 100;

enum VB_TYPES
{
  INDEX_BUFFER,
  POS_VB,
  NORMAL_VB,
  TEXCOORD_VB,
  BONE_VB,
  NUM_VBs
};

struct BoneInfo
{
  Matrix4f BoneOffset;
  Matrix4f FinalTransformation;

  BoneInfo()
  {
    BoneOffset.SetZero();
    FinalTransformation.SetZero();
  }
};

struct VertexBoneData
{
  unsigned IDs[NUM_BONES_PER_VEREX];
  float Weights[NUM_BONES_PER_VEREX];

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

struct MeshEntry
{
  MeshEntry()
  {
    NumIndices = 0;
    BaseVertex = 0;
    BaseIndex = 0;
    MaterialIndex = INVALID_MATERIAL;
  }

  unsigned int NumIndices;
  unsigned int BaseVertex;
  unsigned int BaseIndex;
  unsigned int MaterialIndex;
};

class Model : public ComponentSystem::Component
{
  unsigned m_VAO;
  unsigned m_Buffers[NUM_VBs];

public:
  void Clear();
  Model(std::string &path, Shader &aShaderProgram, bool gammaCorrection);
  void Draw(glm::mat4 &transform);
  Shader &ShaderProgram;

  unsigned NumBones() const
  {
    return m_NumBones;
  }

private:
  std::map<std::string, unsigned> m_BoneMapping;
  std::vector<BoneInfo> m_BoneInfo;
  std::vector<MeshEntry> m_Entries;
  std::vector<Texture *> m_Textures;
  unsigned m_boneLocation[MAX_BONES];

  unsigned m_NumBones;
  std::string directory;
  bool gammaCorrection;

  void BoneTransform(float TimeInSeconds, std::vector<Matrix4f>& Transforms);
  void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4f& ParentTransform);
  const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
  void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
  void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
  void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);  
  bool loadModel(std::string &path);
  bool InitMaterials(const aiScene *pScene, const std::string &Filename);

  unsigned FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
  unsigned FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
  unsigned FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

  void InitMesh(unsigned MeshIndex,
                const aiMesh *paiMesh,
                std::vector<Vector3f> &Positions,
                std::vector<Vector3f> &Normals,
                std::vector<Vector2f> &TexCoords,
                std::vector<VertexBoneData> &Bones,
                std::vector<unsigned> &Indices);

  bool InitFromScene(const aiScene *pScene, const std::string &Filename);

  void LoadBones(unsigned MeshIndex, const aiMesh *pMesh, std::vector<VertexBoneData> &Bones);

  void SetBoneTransform(unsigned Index, const Matrix4f &Transform);

  Matrix4f m_GlobalInverseTransform;
  const aiScene *m_pScene;
  Assimp::Importer m_Importer;

  ComponentSystem::ComponentType GetComponentType() override;
  std::string Serialize()
  {
    return Component::Serialize();
  }
};