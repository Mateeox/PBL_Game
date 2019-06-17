#pragma once
#include "Collider.hpp"
#include "GameObject.hpp"
#include "Game.hpp"

class PlayerCollider : public Collider
{

	Game * game;
public:
		PlayerCollider(Game * game,Transform & transform);

	bool checkCollision(Collider* other)override;

};