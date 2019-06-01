#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Collider.hpp"

//using namespace ComponentSystem;
class GameObject;

class Trigger : public Collider {

protected:
	bool activated;
public:
	Trigger(Transform& transform);
	ComponentSystem::ComponentType GetComponentType() override;
	virtual void ActivateTrigger() = 0;
};