#pragma once
#include "Window.hpp"
#include "SceneNode.hpp"
#include "Component/Model.hpp"
#include "Collider.hpp"
#include "Trigger.hpp"
#include "GuiElement.hpp"
#include "Triggers/TrapTrigger.hpp"
#include "Triggers/EnemyTrigger.hpp"

class Game;
class Player
{
public:
	Player(SceneNode *leftPlayer,
		   SceneNode *rightPlayer,
		   int amountOfParts,
		   Shader &shader,
		   SceneNode *leftScene,
		   SceneNode *rightScene,
		   SceneNode *enemy,
		   SimpleGUI::GuiElement *background,
		   SimpleGUI::GuiElement *win,
		   SimpleGUI::GuiElement *aTrapCollector,
		   Game *game);
	void Update(PBLGame::Window *okienko, float scale);

	int Parts();
	bool Trap();
	void AddTrapPart();
	bool trapSet = false;
	int partsLimit;
	int PartsAmount;
	bool trap_SET_PICK_Enabled = true;

private:
	Game *game;
	SceneNode *leftScene;
	SceneNode *rightScene;
	SceneNode *leftPlayer;
	SceneNode *rightPlayer;
	SceneNode *enemy;
	SimpleGUI::GuiElement *win;
	SimpleGUI::GuiElement *trapCollector;
	SimpleGUI::GuiElement *background;
	Model *trapMod;
	bool isAdding = false;

	SceneNode *CreateTrap(float scale);
	EnemyTrigger *enemyTrigger;
	TrapTrigger *trigger;
};