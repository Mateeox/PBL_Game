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
		for (Component* comp : door->gameObject->components)
		{
			if (comp->GetComponentType() == ComponentType::Collider)
				((Collider*)comp)->Enabled = false;
		}
		door->Scale(0);
		gameobject->transform.ScaleTransform(0, 0, 0);
		activated = true;
	}
}