#pragma once
#include "Window.hpp"
#include "SceneNode.hpp"
#include "Component/Model.hpp"
#include "Collider.hpp"
#include "Trigger.hpp"
#include "GuiElement.hpp"
#include "Triggers/TrapTrigger.hpp"

class Game;
class Player
{
public:
	Player(SceneNode* player, int amountOfParts, Shader& shader, SceneNode* node, SceneNode* enemy,SimpleGUI::GuiElement * background,SimpleGUI::GuiElement * win, Game * game);
	void Update(PBLGame::Window* okienko, float scale);
	int Parts();
	bool Trap();
	void AddTrapPart();
	bool trapSet = false;
	int partsLimit;
	int PartsAmount;
private:
	Game * game;
	SceneNode* parentNode;
	SceneNode* player;
	SceneNode* enemy;
	SimpleGUI::GuiElement * win;
	SimpleGUI::GuiElement * background;
	Model* trapMod;
	bool isAdding = false;


	SceneNode* CreateTrap(float scale);
	Trigger* enemyTrigger;
	Trigger* trigger;
};