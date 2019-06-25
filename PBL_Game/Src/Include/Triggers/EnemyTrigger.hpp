#pragma once
#include "Component/Component.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"
#include "SceneNode.hpp"
#include <iostream>
#include "GuiElement.hpp"
#include "PostProcessing/PostProcessShader.hpp"
//using namespace ComponentSystem;
class GameObject;

class EnemyTrigger : public Trigger {

	SceneNode* Player;
	SimpleGUI::GuiElement *background;
	SimpleGUI::GuiElement *win;
	PostProcessShader* postProcess;
public:
	EnemyTrigger(Transform& transform, SceneNode* player,SimpleGUI::GuiElement * aBackground,SimpleGUI::GuiElement * aWin, PostProcessShader* postProcess);
	bool checkCollision(Collider* other) override;
	void ActivateTrigger() {
		if (!activated) {
			std::cout << "Player killed" << std::endl;
			postProcess->Kill();
			background->SwtichVisiblity();
			win->SwtichVisiblity();

			background->FadeFromTransparent(4);
			win->FadeFromTransparent(3);
			win->FadeToColor(glm::vec3(1,0,0),1);

			//Player->Scale(0.0001);
			activated = true;
		}
	}
};