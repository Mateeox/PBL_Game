#include <iostream>
#include "EnemyTrigger.hpp"

using namespace ComponentSystem;

EnemyTrigger::EnemyTrigger(Transform & transform, SceneNode *leftPlayerNode) : Trigger(transform)
{
	this->leftPlayerNode = leftPlayerNode;
}

void EnemyTrigger::ActivateTrigger()
{
	if (!activated)
	{
		std::cout << "Potwor zlapal gracza" << std::endl;
		activated = true;
		leftPlayerNode->Scale(0, 0, 0);
	}
}