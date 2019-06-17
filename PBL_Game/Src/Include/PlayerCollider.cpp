#include "PlayerCollider.hpp"


PlayerCollider::PlayerCollider(Transform & transform) : Collider(transform)
{

}

bool PlayerCollider::checkCollision(Collider* other)
{
	if (Enabled) {


		if (other->gameobject->getTag() != "enemy" && other->gameobject->getTag() != "trap")
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
	}
	else {
		return false;
	}

}