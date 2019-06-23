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
		door->local.Translate(glm::vec3(0, -500.0f, 0));
		gameobject->node->local.Translate(glm::vec3(0, -500.0f, 0));
		activated = true;
	}
}