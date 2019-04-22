#pragma once
#include "Game.hpp"

class PlayerMovementScript
{
public:
	PlayerMovementScript(GameObject *left, GameObject *right);
	~PlayerMovementScript();

	void Start();
	void Update();

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

private:
	void SetCurrent();

	float offset = 1.0f;
	GameObject *leftPlayer, *rightPlayer, *current;
};

