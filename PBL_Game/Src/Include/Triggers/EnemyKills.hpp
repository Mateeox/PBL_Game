#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>

//using namespace ComponentSystem;
class GameObject;

class EnemyKills : public Trigger {

	SceneNode* Player;
public:
	EnemyKills(Transform& transform, SceneNode* player);
	void ActivateTrigger() {
		if (!activated) {
			std::cout << "Player killed" << std::endl;
			Player->Scale(0);
			activated = true;
		}
	}
};