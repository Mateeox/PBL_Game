#pragma once
#include "Transform.hpp"
#include "Component/Drawable.hpp"
#include <vector>
#include "SceneNode.hpp"

#include "Component/Component.hpp"

class SceneNode;
class GameObject
{
	std::string tag;
  public:
    Transform &transform;
    std::vector<ComponentSystem::Component *> components;
    SceneNode * node;
    GameObject(Transform &transform);
    ComponentSystem::Component *GetComponent(ComponentSystem::ComponentType type);
    void AddComponent(ComponentSystem::Component *);
    void RemoveComponent(ComponentSystem::ComponentType type);
	std::string Serialize();

  void Destroy();

	void setTag(std::string tag);
	std::string getTag();
};
