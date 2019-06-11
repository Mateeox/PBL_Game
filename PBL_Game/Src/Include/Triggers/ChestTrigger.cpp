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
		player->AddTrap();
		transform.ScaleTransform(0, 0, 0);
		activated = true;
	}
}