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

    std::vector<SceneNode *> childres;
    GameObject *gameObject;

    SceneNode();
    SceneNode(GameObject *gameObject);

    void AddChild(SceneNode *aSceneNode);
    void AddGameObject(GameObject *aGameObject);
    void Render(Transform& parentWorld, bool aDirty_Flag);

    void Scale(float x, float y, float z);
    void Translate(float x, float y, float z);
    void Rotate(float x, glm::vec3 axis);
};