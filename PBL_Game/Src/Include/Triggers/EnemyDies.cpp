#include "EnemyDies.hpp"

EnemyDies::EnemyDies(Transform& transform, SceneNode* enemy) : Trigger(transform)
{
	this->Enemy = enemy;
}