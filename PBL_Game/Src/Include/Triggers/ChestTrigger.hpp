#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include "Player.hpp"

//using namespace ComponentSystem;
class GameObject;

class ChestTrigger : public Trigger {
private:
	Player* player;
public:
	ChestTrigger(Transform& transform, Player* player);
	virtual void ActivateTrigger();
};