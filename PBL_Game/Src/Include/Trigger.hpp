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
	void SetActivated(bool status);
	Trigger(Transform& transform);
	virtual bool checkCollision(Collider* other) = 0;
	ComponentSystem::ComponentType GetComponentType() override;
	virtual void ActivateTrigger() = 0;
};