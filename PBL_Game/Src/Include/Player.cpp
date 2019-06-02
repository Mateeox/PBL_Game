#include "Player.hpp"

Player::Player(SceneNode* aPlayer, int aPartsLimit, Shader& aShader, SceneNode* aNode)
{
	parentNode = aNode;
	player = aPlayer;
	partsLimit = 0;
	PartsAmount = 0;
	placingTrap = false;
	trapMod = new Model("Models/Trap/TrapPart_JawHinges.obj", aShader, false);
}

int Player::Parts()
{
	return PartsAmount;
}

bool Player::Trap()
{
	return PartsAmount == partsLimit;
}

void Player::AddTrap()
{
	PartsAmount++;
}

void Player::Update(PBLGame::Window* okienko, float scale)
{
	if (glfwGetKey(okienko->window, GLFW_KEY_SPACE) == GLFW_PRESS && !placingTrap && Trap())
	{
		placingTrap = true;
		PartsAmount -= partsLimit;
		parentNode->AddChild(CreateTrap(scale));
	}
}

SceneNode* Player::CreateTrap(float scale)
{
	SceneNode* trap = new SceneNode();
	GameObject* trapObj = new GameObject(trap->local);
	trapObj->AddComponent(trapMod);
	trap->AddGameObject(trapObj);
	glm::vec3 Position = player->local.getPosition();
	trap->Scale(scale);
	trap->Translate(Position.x*6, 0, Position.z*6);
	//floor->Rotate(-90.0f, glm::vec3(1, 0, 0));
	return std::move(trap);
}