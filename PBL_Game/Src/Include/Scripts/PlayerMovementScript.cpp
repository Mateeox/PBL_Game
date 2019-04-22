#include "PlayerMovementScript.h"

PlayerMovementScript::PlayerMovementScript(GameObject *left, GameObject *right)
{
	leftPlayer = left;
	rightPlayer = right;
}

PlayerMovementScript::~PlayerMovementScript()
{
}

void PlayerMovementScript::Start()
{
}

void PlayerMovementScript::Update()
{
}

void PlayerMovementScript::MoveLeft()
{
	SetCurrent();
	
	current->transform.Translate(vec3(-offset, 0, 0));
}

void PlayerMovementScript::MoveRight()
{
	SetCurrent();

	current->transform.Translate(vec3(offset, 0, 0));
}

void PlayerMovementScript::MoveUp()
{
	SetCurrent();

	current->transform.Translate(vec3(0, 0, -offset));
}

void PlayerMovementScript::MoveDown()
{
	SetCurrent();

	current->transform.Translate(vec3(0, 0, offset));
}

void PlayerMovementScript::SetCurrent()
{
	if (leftSideActive)
		current = leftPlayer;
	else
		current = rightPlayer;
}