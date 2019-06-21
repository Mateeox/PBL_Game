#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>
#include "GuiElement.hpp"

//using namespace ComponentSystem;
class GameObject;

class TrapTrigger : public Trigger {

	SceneNode* enemy;
	SimpleGUI::GuiElement * background;
	SimpleGUI::GuiElement * lostScreen;
	
public:
	TrapTrigger(Transform& transform, SceneNode* enemy,SimpleGUI::GuiElement * background,
	SimpleGUI::GuiElement * lost);
	bool checkCollision(Collider* other) override;
	void ActivateTrigger() {
		if (!activated) 
		{
			background->SwtichVisiblity();
			lostScreen->SwtichVisiblity();

			background->FadeFromTransparent(2);
			lostScreen->FadeFromTransparent(1);
			lostScreen->FadeToColor(glm::vec3(1,0,0),1);

			std::cout << "Enemy killed" << std::endl;
			enemy->Scale(0.0001);
			activated = true;
		}
	}
};