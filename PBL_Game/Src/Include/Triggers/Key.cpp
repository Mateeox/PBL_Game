#include <iostream>
#include "Key.hpp"

using namespace ComponentSystem;

Key::Key(Transform & transform, SceneNode* door) : Trigger(transform)
{
	this->door = door;
}

bool Key::checkCollision(Collider* other)
{
	if (Enabled) {
		//NOT IMPLEMENTED
		return true;
	}
	else {
		return false;
	}

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
		door->gameObject->transform.ScaleTransform(0, 0, 0);
		gameobject->transform.ScaleTransform(0, 0, 0);
		activated = true;
	}
}