#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>
#include "GuiElement.hpp"
//using namespace ComponentSystem;
class GameObject;

class EnemyTrigger : public Trigger {

	SceneNode* Enemy;
	SimpleGUI::GuiElement *background;
	SimpleGUI::GuiElement *win;
public:
	EnemyTrigger(Transform& transform, SceneNode* enemy,SimpleGUI::GuiElement * aBackground,SimpleGUI::GuiElement * aWin);
	bool checkCollision(Collider* other) override;
	void ActivateTrigger() {
		if (!activated) {
			std::cout << "Player killed" << std::endl;
			background->SwtichVisiblity();
			win->SwtichVisiblity();

			background->FadeFromTransparent(4);
			win->FadeFromTransparent(3);
			win->FadeToColor(glm::vec3(1,0,0),1);

			Enemy->Scale(0.0001);
			activated = true;
		}
	}
};