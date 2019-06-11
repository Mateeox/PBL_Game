#include "Player.hpp"
#include "Triggers/EnemyDies.hpp"

Player::Player(SceneNode* aPlayer, int aPartsLimit, Shader& aShader, SceneNode* aNode)
{
	parentNode = aNode;
	player = aPlayer;
	partsLimit = 0;
	PartsAmount = 0;
	placingTrap = false;
	trapMod = new Model("Models/Trap/Trap_Anim.fbx", aShader, false);
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
	EnemyDies* trapTrig = new EnemyDies(trap->local, nullptr);			//dodaæ przeciwnika
	trapObj->AddComponent(trapMod);
	trapObj->AddComponent(trapTrig); 
	trap->AddGameObject(trapObj);
	glm::vec3 Position = player->local.getPosition();
	trap->Scale(scale);
	trap->Translate(Position.x*6, 0, Position.z*6);
	//trap->Rotate(90.0f, glm::vec3(1, 0, 0));
	return std::move(trap);
}