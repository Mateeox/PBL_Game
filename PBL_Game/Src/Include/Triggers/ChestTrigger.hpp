#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"

//using namespace ComponentSystem;
class GameObject;

class ChestTrigger : public Trigger {
//private:
//	SceneNode* chest;
public:
	ChestTrigger(Transform& transform);
	virtual void ActivateTrigger();
};