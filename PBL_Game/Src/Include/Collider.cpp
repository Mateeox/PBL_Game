#include "Collider.hpp"

using namespace ComponentSystem;

Collider::Collider(Transform & transform) : transform(transform)
{
}

ComponentType Collider::GetComponentType()
{
	return ComponentType::Collider;
}

bool Collider::checkCollision(Collider* other)
{
	glm::vec3 translatedCoords = coords + transform.getPosition() * transform.getScale();
	glm::vec3 otherTranslatedCoords = other->coords + other->transform.getPosition() * other->transform.getScale();
	return
		(translatedCoords.x <= otherTranslatedCoords.x + other->dimensions.x) && (translatedCoords.x + dimensions.x >= otherTranslatedCoords.x) &&
		(translatedCoords.y <= otherTranslatedCoords.y + other->dimensions.y) && (translatedCoords.y + dimensions.y >= otherTranslatedCoords.y) &&
		(translatedCoords.z <= otherTranslatedCoords.z + other->dimensions.z) && (translatedCoords.z + dimensions.z >= otherTranslatedCoords.z);
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
