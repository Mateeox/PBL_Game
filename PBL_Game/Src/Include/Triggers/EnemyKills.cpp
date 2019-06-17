#include "EnemyKills.hpp"

EnemyKills::EnemyKills(Transform& transform, SceneNode* player,SimpleGUI::GuiElement * aBack,SimpleGUI::GuiElement * aLost) : 
Trigger(transform),background(aBack),lostScreen(aLost)
{
	this->Player = player;
}

bool EnemyKills::checkCollision(Collider* other)
{

	return false;
}