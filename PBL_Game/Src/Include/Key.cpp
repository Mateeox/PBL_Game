#include <iostream>
#include "Key.hpp"

using namespace ComponentSystem;

Key::Key(Transform & transform, Door* door) : Trigger(transform)
{
	this->door = door;
}

void Key::ActivateTrigger()
{
	if (!activated)
	{
		std::cout << "Podniesiono klucz" << std::endl;
		activated = true;
		door->Open();
	}
}