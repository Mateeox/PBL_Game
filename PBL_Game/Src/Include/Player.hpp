#pragma once
#include "Window.hpp"
#include "SceneNode.hpp"
#include "Component/Model.hpp"

class Player
{
public:
	Player(SceneNode* player, int amountOfParts, Shader& shader, SceneNode* node);
	void Update(PBLGame::Window* okienko, float scale);
	int Parts();
	bool Trap();
	void AddTrap();
private:
	SceneNode* parentNode;
	SceneNode* player;
	Model* trapMod;
	int partsLimit;
	int PartsAmount;
	bool placingTrap;
	SceneNode* CreateTrap(float scale);
};