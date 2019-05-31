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

class AnimatedModel : public ComponentSystem::Component
{
  unsigned m_VAO;
  unsigned m_Buffers[NUM_VBs];
  bool m_Animate;
  unsigned m_AnimationNubmer;

  float _animationTime0;
  float _animationTime1;

  float _blendingTime;
  float _blendingTimeMul;
  uint _prevAnimIndex;

  long long m_lastTime;
  bool _updateBoth;
  bool _temporary;
  float _playTime;

  void Update();

public:
  void Clear();
  AnimatedModel(std::string &path, Shader &aShaderProgram, bool gammaCorrection);
  void Draw(glm::mat4 &transform);
  Shader &ShaderProgram;
  void SelectAnimation(const std::string &aName);
  void SelectAnimation(unsigned aNumer);
  unsigned GetAnimationNR();
  std::string GetAnimationName();
  bool SetAnimIndex(uint index, bool updateBoth = true, float blendDuration = 0.3f, bool temporary = false, float time = 0.f);
  void ListAnimationNames();

  unsigned NumBones() const
  {
    return m_NumBones;
  }

private:
  aiVector3D _rotation;
  std::map<std::string, unsigned> m_BoneMapping;
  std::vector<BoneInfo> m_BoneInfo;
  std::vector<MeshEntry> m_Entries;
  std::vector<Texture *> m_Textures;
  unsigned m_boneLocation[MAX_BONES];

  unsigned m_NumBones;
  std::string directory;
  bool gammaCorrection;

  void BoneTransform(float TimeInSeconds, std::vector<Matrix4f> &Transforms);

   void ReadNodeHeirarchy(const aiScene *scene0,
                         const aiScene *scene1,
                         float AnimationTime0,
                         float AnimationTime1,
                         const aiNode *pNode0,
                         const aiNode *pNode1,
                         const aiMatrix4x4 &ParentTransform,
                         int stopAnimLevel);
                         
  void ReadNodeHeirarchy(const aiScene *scene,
                         float AnimationTime,
                         const aiNode *pNode,
                         const Matrix4f &ParentTransform,
                         int stopAnimLevel);

  const aiNodeAnim *FindNodeAnim(const aiAnimation *pAnimation, const std::string NodeName);
 
  void CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);
  void CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);
  void CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim);
  bool loadAnimatedModel(std::string &path);
  bool InitMaterials(const aiScene *pScene, const std::string &Filename);

  unsigned FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim);
  unsigned FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim);
  unsigned FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim);

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

  void SetRotation(const aiVector3D &r) { _rotation = r; }
  Matrix4f m_GlobalInverseTransform;
  const aiScene *m_pScene;
  Assimp::Importer m_Importer;

  std::vector<const aiScene *> _scenes;
  uint _curScene;

  ComponentSystem::ComponentType GetComponentType() override;
  std::string Serialize()
  {
    return Component::Serialize();
  }
};