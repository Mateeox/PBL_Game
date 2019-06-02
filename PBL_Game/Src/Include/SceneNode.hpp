#pragma once
#include "GameObject.hpp"
#include "Transform.hpp"
#include <vector>

class SceneNode
{

    
    int numChildren;
    bool dirty_flag;

  public:

    Transform local;
    Transform world;

	SceneNode* parent;
    std::vector<SceneNode *> children;
    GameObject *gameObject;

    SceneNode();
    SceneNode(GameObject *gameObject);

	void AddParent(SceneNode *aSceneNode);
    void AddChild(SceneNode *aSceneNode);
    void AddGameObject(GameObject *aGameObject);
    void Render(Transform& parentWorld, bool aDirty_Flag);

    void Scale(float x, float y, float z);
    void Scale(float value);
    void Translate(float x, float y, float z);
    void DynamicTranslate(float interpolation,float x,float y,float z);
    void Rotate(float x, glm::vec3 axis);
	
	std::string Serialize();
};