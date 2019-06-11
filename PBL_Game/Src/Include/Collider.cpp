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

bool Collider::checkCollision(Collider* other)
{
	if (Enabled) {

		if (other->gameobject->getTag() != "enemy")
		{
			glm::vec3 translatedCoords = coords + transform.getPosition() * transform.getScale();
			glm::vec3 otherTranslatedCoords{ 0 };
			otherTranslatedCoords.x = other->coords.x + other->transform.getPosition().x;// *other->transform.getScale().x;
			otherTranslatedCoords.z = other->coords.z + other->transform.getPosition().z; //* other->transform.getScale().z;

			if ((translatedCoords.x <= otherTranslatedCoords.x + other->dimensions.x) && (translatedCoords.x + dimensions.x >= otherTranslatedCoords.x) &&
				(translatedCoords.z <= otherTranslatedCoords.z + other->dimensions.z) && (translatedCoords.z + dimensions.z >= otherTranslatedCoords.z))
			{

				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			
				glm::vec3 translatedCoords = coords + transform.getPosition() * transform.getScale();
				glm::vec3 otherTranslatedCoords = other->coords + other->transform.getPosition() * other->transform.getScale();
				return
					(translatedCoords.x <= otherTranslatedCoords.x + other->dimensions.x) && (translatedCoords.x + dimensions.x >= otherTranslatedCoords.x) &&
					(translatedCoords.y <= otherTranslatedCoords.y + other->dimensions.y) && (translatedCoords.y + dimensions.y >= otherTranslatedCoords.y) &&
					(translatedCoords.z <= otherTranslatedCoords.z + other->dimensions.z) && (translatedCoords.z + dimensions.z >= otherTranslatedCoords.z);
		}
	} else {
		return true;
	}

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
