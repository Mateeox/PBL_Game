#include <iostream>
#include "Key.hpp"

using namespace ComponentSystem;

Key::Key(Transform & transform, SceneNode* door) : Trigger(transform)
{
	this->door = door;
}

void Key::ActivateTrigger()
{
	if (!activated)
	{
		std::cout << "Podniesiono klucz" << std::endl;
		door->RemoveGameObject();
		gameobject->Destroy();
	}
}