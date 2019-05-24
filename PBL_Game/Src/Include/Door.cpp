#include <iostream>
#include "Door.hpp"

using namespace ComponentSystem;

Door::Door(Transform & transform, SceneNode* node) : Collider(transform)
{
	this->node = node;
}

void Door::Open()
{
	node->Scale(2, 1, 0.5);
}