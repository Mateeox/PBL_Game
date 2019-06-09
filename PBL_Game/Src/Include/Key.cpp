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
		transform.Translate(glm::vec3(0, 100.0f, 0));
		door->Scale(3, 1, 0.2);
		activated = true;
	}
}