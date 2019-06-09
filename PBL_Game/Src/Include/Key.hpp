#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "Door.hpp"

//using namespace ComponentSystem;
class GameObject;

class Key : public Trigger {

	SceneNode* door;

public:
	Key(Transform& transform, SceneNode* door);
	virtual void ActivateTrigger();
};