#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>

//using namespace ComponentSystem;
class GameObject;

class EnemyDies : public Trigger {

	SceneNode* Enemy;
public:
	EnemyDies(Transform& transform, SceneNode* enemy);
	void ActivateTrigger() {
		if (!activated) {
			std::cout << "Enemy killed" << std::endl;
			Enemy->Scale(0);
			activated = true;
		}
	}
};