#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"

//using namespace ComponentSystem;
class GameObject;

class Door : public Collider {

	SceneNode* node;
public:
	Door(Transform& transform, SceneNode* node);
	void Open();
};