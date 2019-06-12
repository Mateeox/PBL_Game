#include "EnemyDies.hpp"

EnemyDies::EnemyDies(Transform& transform, SceneNode* enemy,SimpleGUI::GuiElement * aBackground,SimpleGUI::GuiElement * aWin) : Trigger(transform),background(aBackground),win(aWin)
{
	this->Enemy = enemy;
}