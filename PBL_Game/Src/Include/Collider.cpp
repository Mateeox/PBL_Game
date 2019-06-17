#include "Collider.hpp"
#include "GameObject.hpp"

using namespace ComponentSystem;

Collider::Collider(Transform & transform) : transform(transform), Component(nullptr)
{
}

ComponentType Collider::GetComponentType()
{
	return ComponentType::Collider;
}

void Collider::setDimensions(double xOffset, double yOffset, double zOffset, double width, double height, double depth)
{
	//we want the coords to point to the left-down-back vertex
	coords.x = xOffset - width / 2.0;
	coords.y = yOffset - height / 2.0;
	coords.z = zOffset - depth / 2.0;
	dimensions.x = width;
	dimensions.y = height;
	dimensions.z = depth;
}
