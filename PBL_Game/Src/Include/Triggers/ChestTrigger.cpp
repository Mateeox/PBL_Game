#include <iostream>
#include "ChestTrigger.hpp"

using namespace ComponentSystem;

ChestTrigger::ChestTrigger(Transform & transform) : Trigger(transform)
{
}

void ChestTrigger::ActivateTrigger()
{
	if (!activated)
	{
		std::cout << "Otwieranie skrzyni" << std::endl;
		/*chest->RemoveGameObject();
		gameobject->Destroy();*/
	}
}