#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"

//using namespace ComponentSystem;
class GameObject;

class Collider : public ComponentSystem::Component {

public:
	glm::vec3 coords;
	glm::vec3 dimensions;
	bool Enabled = true;
	Transform& transform;
	Collider(Transform& transform);
	ComponentSystem::ComponentType GetComponentType() override;
	virtual bool checkCollision(Collider* other);
	void setDimensions(double xOffset, double yOffset, double zOffset, double width, double height, double depth);
};