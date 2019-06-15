#include "Trigger.hpp"

using namespace ComponentSystem;

Trigger::Trigger(Transform & transform) : Collider(transform)
{
	activated = false;
}

ComponentType Trigger::GetComponentType()
{
	return ComponentType::Trigger;
}

void Trigger::SetActivated(bool status)
{
	activated = status;
}

void Trigger::ActivateTrigger()
{
}
