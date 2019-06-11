#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"

//using namespace ComponentSystem;
class GameObject;

class Key : public Trigger {
private:
	SceneNode* door;
public:
	Key(Transform& transform, SceneNode* door);
	virtual void ActivateTrigger();
};