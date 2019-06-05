#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "AnimatedModel.hpp"
#include <iostream>
#include <string>
#include <cstring>


#include <glm/gtc/type_ptr.hpp>

#define SAFE_DELETE(p) \
	if (p)             \
	{                  \
		delete p;      \
		p = NULL;      \
	}

#define GLCheckError() (glGetError() == GL_NO_ERROR)

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

AnimatedModel::AnimatedModel(std::string &path, Shader &aShaderProgram, bool gammaCor = false) : Component(nullptr),
																								 ShaderProgram(aShaderProgram),
																								 m_Animate(true),
																								 m_AnimationNubmer(0),
																								 gammaCorrection(gammaCor)
{
	m_NumBones = 0;
	loadAnimatedModel(path);
}

ComponentSystem::ComponentType AnimatedModel::GetComponentType()
{
	return ComponentSystem::ComponentType::AnimatedModel;
}

bool AnimatedModel::loadAnimatedModel(std::string &path)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool Ret = false;
	
	m_pScene = m_Importer.ReadFile(path.c_str(), ASSIMP_LOAD_FLAGS);

	if (m_pScene)
	{
		m_GlobalInverseTransform = m_pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();
		Ret = InitFromScene(m_pScene, path);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", path.c_str(), m_Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
		m_boneLocation[i] = ShaderProgram.GetUniformLocation(Name);
	}

	return Ret;
}

bool AnimatedModel::InitFromScene(const aiScene *pScene, const std::string &Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);

	std::vector<Vector3f> Positions;
	std::vector<Vector3f> Normals;
	std::vector<Vector2f> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<unsigned> Indices;

	unsigned NumVertices = 0;
	unsigned NumIndices = 0;

	// Count the number of vertices and indices
	for (unsigned i = 0; i < m_Entries.size(); i++)
	{
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned i = 0; i < m_Entries.size(); i++)
	{
		const aiMesh *paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	if (!InitMaterials(pScene, Filename))
	{
		return false;
	}

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid *)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid *)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return GLCheckError();
}

bool AnimatedModel::InitMaterials(const aiScene *pScene, const std::string &Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial *pMaterial = pScene->mMaterials[i];

		m_Textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
			{
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				if (p.c_str()[0] == '*')
				{
					m_Textures[i] = new Texture(p.c_str(), GL_LINEAR, pScene->mTextures);
				}
				else
				{
					m_Textures[i] = new Texture(FullPath.c_str(), GL_LINEAR);
				}

				if (m_Textures[i]->Load())
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else
				{
					printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}

void AnimatedModel::LoadBones(unsigned MeshIndex, const aiMesh *pMesh, std::vector<VertexBoneData> &Bones)
{
	for (unsigned i = 0; i < pMesh->mNumBones; i++)
	{
		unsigned BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
		{
			unsigned VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

void VertexBoneData::AddBoneData(unsigned BoneID, float Weight)
{
	for (unsigned i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++)
	{

		if (Weights[i] == 0.0)
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}

const aiNodeAnim *AnimatedModel::FindNodeAnim(const aiAnimation *pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
	{
		const aiNodeAnim *pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

unsigned AnimatedModel::FindRotation(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

unsigned AnimatedModel::FindScaling(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

void AnimatedModel::CalcInterpolatedRotation(aiQuaternion &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion &StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion &EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void AnimatedModel::CalcInterpolatedScaling(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D &Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D &End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

unsigned AnimatedModel::FindPosition(float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	for (unsigned i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
		{
			return i;
		}
	}

	assert(0);

	return 0;
}

void AnimatedModel::CalcInterpolatedPosition(aiVector3D &Out, float AnimationTime, const aiNodeAnim *pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D &Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D &End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void AnimatedModel::ReadNodeHeirarchy(float AnimationTime, const aiNode *pNode, const Matrix4f &ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation *pAnimation = m_pScene->mAnimations[m_AnimationNubmer];

	Matrix4f NodeTransformation(pNode->mTransformation);

	const aiNodeAnim *pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		Matrix4f ScalingM;
		ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		Matrix4f TranslationM;
		TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	Matrix4f GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void AnimatedModel::Draw(glm::mat4 &transform)
{

	ShaderProgram.use();
	std::vector<Matrix4f> Transforms;

	float RunningTime = glfwGetTime();

	BoneTransform(RunningTime, Transforms);

	for (unsigned int i = 0; i < Transforms.size(); i++)
	{
		SetBoneTransform(i, Transforms[i]);
	}

	
	//Set transform
	unsigned transformLoc = glGetUniformLocation(ShaderProgram.shaderProgramID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(m_VAO);

	for (unsigned i = 0; i < m_Entries.size(); i++)
	{
		const unsigned MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex])
		{
			m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
								 m_Entries[i].NumIndices,
								 GL_UNSIGNED_INT,
								 (void *)(sizeof(unsigned) * m_Entries[i].BaseIndex),
								 m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void AnimatedModel::SelectAnimation(const std::string &aName)
{
	for (unsigned i = 0; i < m_pScene->mNumAnimations; i++)
	{
		if (aName == m_pScene->mAnimations[i]->mName.C_Str())
		{
			m_AnimationNubmer = i;
			return;
		}
	}
}

void AnimatedModel::ListAnimationNames()
{
	for (unsigned i = 0; i < m_pScene->mNumAnimations; i++)
	{
		std::cout << "Animation : " << i << " Name: " << m_pScene->mAnimations[i]->mName.C_Str() << "\n";
	}
}
void AnimatedModel::SelectAnimation(unsigned aNumber)
{
	m_AnimationNubmer = aNumber;
}

void AnimatedModel::BoneTransform(float TimeInSeconds, std::vector<Matrix4f> &Transforms)
{
	Matrix4f Identity;
	Identity.InitIdentity();

	float TicksPerSecond = (float)(m_pScene->mAnimations[m_AnimationNubmer]->mTicksPerSecond != 0 ? m_pScene->mAnimations[m_AnimationNubmer]->mTicksPerSecond : 25.0f);
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[m_AnimationNubmer]->mDuration);

	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);

	for (unsigned int i = 0; i < m_NumBones; i++)
	{
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

void AnimatedModel::InitMesh(unsigned MeshIndex,
							 const aiMesh *paiMesh,
							 std::vector<Vector3f> &Positions,
							 std::vector<Vector3f> &Normals,
							 std::vector<Vector2f> &TexCoords,
							 std::vector<VertexBoneData> &Bones,
							 std::vector<unsigned> &Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D *pPos = &(paiMesh->mVertices[i]);
		const aiVector3D *pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D *pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
		Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
	}

	LoadBones(MeshIndex, paiMesh, Bones);

	// Populate the index buffer
	for (unsigned i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace &Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

void AnimatedModel::SetBoneTransform(unsigned Index, const Matrix4f &Transform)
{
	assert(Index < MAX_BONES);
	//Transform.Print();
	glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, (const GLfloat *)Transform);
}

void AnimatedModel::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		SAFE_DELETE(m_Textures[i]);
	}

	if (m_Buffers[0] != 0)
	{
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

unsigned AnimatedModel::GetAnimationNR()
{
	return m_AnimationNubmer;
}
std::string AnimatedModel::GetAnimationName()
{
	return m_pScene->mAnimations[m_AnimationNubmer]->mName.C_Str();
}