#include "EnemyTrigger.hpp"

EnemyTrigger::EnemyTrigger(Transform& transform, SceneNode* player,SimpleGUI::GuiElement * aBack,SimpleGUI::GuiElement * aLost) : 
Trigger(transform),background(aBack),lostScreen(aLost)
{
	this->Player = player;
}

bool EnemyTrigger::checkCollision(Collider* other)
{
	if (Enabled) {

			glm::vec3 translatedCoords = coords + transform.getPosition() * transform.getScale();
			glm::vec3 otherTranslatedCoords = other->coords + other->transform.getPosition() * other->transform.getScale();
			return
				(translatedCoords.x <= otherTranslatedCoords.x + other->dimensions.x) && (translatedCoords.x + dimensions.x >= otherTranslatedCoords.x) &&
				(translatedCoords.y <= otherTranslatedCoords.y + other->dimensions.y) && (translatedCoords.y + dimensions.y >= otherTranslatedCoords.y) &&
				(translatedCoords.z <= otherTranslatedCoords.z + other->dimensions.z) && (translatedCoords.z + dimensions.z >= otherTranslatedCoords.z);
		
	}
}