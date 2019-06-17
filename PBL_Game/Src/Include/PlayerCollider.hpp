#pragma once
#include "Collider.hpp"
#include "GameObject.hpp"

class PlayerCollider : public Collider
{

public:
		PlayerCollider(Transform & transform);

	bool checkCollision(Collider* other)override;

};