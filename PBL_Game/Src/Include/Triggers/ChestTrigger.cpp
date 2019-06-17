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
		transform.ScaleTransform(0, 0, 0);
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