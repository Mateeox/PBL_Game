#include "Player.hpp"

Player::Player(SceneNode* player, int partsLimit, Shader* shader, std::vector<SceneNode*> nodes)
{
	this->nodes = nodes;
	this->player = player;
	this->partsLimit = partsLimit;
	this->partsLimit = 0;
	this->PartsAmount = 0;
	this->placingTrap = false;
	trapMod = new Model("Models/Trap/TrapPart_JawHinges.obj", *shader, false);
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
		nodes.push_back(CreateTrap(scale));
		placingTrap = false;
	}
}

SceneNode* Player::CreateTrap(float scale)
{
	SceneNode* trap = new SceneNode();
	GameObject* trapObj = new GameObject(trap->local);
	trapObj->AddComponent(trapMod);
	trap->AddGameObject(trapObj);
	glm::vec3 Position = player->local.getPosition();
	trap->Translate(Position.x * scale, 0, Position.y * scale);
	//floor->Rotate(-90.0f, glm::vec3(1, 0, 0));
	//trap->Scale(scale);
	return trap;
}