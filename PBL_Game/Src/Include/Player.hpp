#pragma once
#include "Window.hpp"
#include "SceneNode.hpp"
#include "Component/Model.hpp"
#include "Collider.hpp"
#include "Trigger.hpp"
#include "GuiElement.hpp"

class Player
{
public:
	Player(SceneNode* player, int amountOfParts, Shader& shader, SceneNode* node, SceneNode* enemy,SimpleGUI::GuiElement * background,SimpleGUI::GuiElement * win);
	void Update(PBLGame::Window* okienko, float scale);
	int Parts();
	bool Trap();
	void AddTrap();
	bool trapSet = false;
	int partsLimit;
	int PartsAmount;
private:
	SceneNode* parentNode;
	SceneNode* player;
	SceneNode* enemy;
	SimpleGUI::GuiElement * win;
	SimpleGUI::GuiElement * background;
	Model* trapMod;



	SceneNode* CreateTrap(float scale);
	Trigger* enemyTrigger;
	Trigger* trigger;
};