#include "EnemyKills.hpp"

EnemyKills::EnemyKills(Transform& transform, SceneNode* player) : Trigger(transform)
{
	this->Player = player;
}