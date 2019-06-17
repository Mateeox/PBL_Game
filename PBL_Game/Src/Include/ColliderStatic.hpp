#pragma once

#include "Collider.hpp"
#include "GameObject.hpp"

class ColliderStatic : public Collider
{

public:
	ColliderStatic(Transform & transform);

	bool checkCollision(Collider* other)override;

};