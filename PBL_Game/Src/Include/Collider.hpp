#pragma once
#include "Component.hpp"
#include "Transform.hpp"

//using namespace ComponentSystem;
class GameObject;

class Collider : public ComponentSystem::Component {

	glm::vec3 coords;
	glm::vec3 dimensions;

public:
	Transform& transform;
	Collider(Transform& transform);
	ComponentSystem::ComponentType GetComponentType() override;
	virtual bool checkCollision(Collider* other);
	void setDimensions(double xOffset, double yOffset, double zOffset, double width, double height, double depth);
};