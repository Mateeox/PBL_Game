#include <iostream>
#include "ChestTrigger.hpp"

using namespace ComponentSystem;

ChestTrigger::ChestTrigger(Transform & transform, Player* player) : Trigger(transform)
{
	this->player = player;
}

void ChestTrigger::ActivateTrigger()
{
	if (!activated)
	{
		std::cout << "Otwieranie skrzyni" << std::endl;
		player->AddTrapPart();
		glm::vec3 pos = gameobject->transform.getPosition();
		gameobject->transform.Translate(glm::vec3(0, -500.0f, 0));
		activated = true;
	}
}

bool ChestTrigger::checkCollision(Collider* other)
{
	if (Enabled) {
		 //NOT IMPLEMENTED
		return true;
	}
	else {
		return false;
	}

}