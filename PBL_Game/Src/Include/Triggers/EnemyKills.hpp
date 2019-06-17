#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>
#include "GuiElement.hpp"

//using namespace ComponentSystem;
class GameObject;

class EnemyKills : public Trigger {

	SceneNode* Player;
	SimpleGUI::GuiElement * background;
	SimpleGUI::GuiElement * lostScreen;
	
public:
	EnemyKills(Transform& transform, SceneNode* player,SimpleGUI::GuiElement * background,
	SimpleGUI::GuiElement * lost);
	void ActivateTrigger() {
		if (!activated) 
		{
			background->SwtichVisiblity();
			lostScreen->SwtichVisiblity();

			background->FadeFromTransparent(2);
			lostScreen->FadeFromTransparent(1);
			lostScreen->FadeToColor(glm::vec3(1,0,0),1);

			std::cout << "Player killed" << std::endl;
			//Player->Scale(0);
			activated = true;
		}
	}
};