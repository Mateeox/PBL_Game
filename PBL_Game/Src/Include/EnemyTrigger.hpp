#pragma once
#include "Component/Component.hpp"
#include "SceneNode.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"

//using namespace ComponentSystem;
class GameObject;

class EnemyTrigger : public Trigger {

	SceneNode *leftPlayerNode;

public:
	EnemyTrigger(Transform& transform, SceneNode *leftPlayerNode);
	virtual void ActivateTrigger();
};