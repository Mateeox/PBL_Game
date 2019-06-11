#include "Player.hpp"
#include "Triggers/EnemyDies.hpp"
#include "Component/Component.hpp"

Player::Player(SceneNode* aPlayer, int aPartsLimit, Shader& aShader, SceneNode* aNode, SceneNode* enemy)
{
	parentNode = aNode;
	player = aPlayer;
	this->enemy = enemy;
	partsLimit = aPartsLimit;
	PartsAmount = 0;
	trapMod = new Model("Models/Trap/Trap_Anim.fbx", aShader, false);
	enemyCollider = nullptr;
	trigger = nullptr;
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
	if (glfwGetKey(okienko->window, GLFW_KEY_SPACE) == GLFW_PRESS && Trap())
	{
		PartsAmount -= partsLimit;
		parentNode->AddChild(CreateTrap(scale));
	}
	else if(enemyCollider != nullptr && trigger != nullptr)
	{
		if (enemyCollider->checkCollision(trigger))
			trigger->ActivateTrigger();
	}
}

SceneNode* Player::CreateTrap(float scale)
{
	SceneNode* trap = new SceneNode();
	GameObject* trapObj = new GameObject(trap->local);
	trigger = new EnemyDies(trap->local, enemy);			//dodaæ przeciwnika
	trigger->setDimensions(0, 0, 0, 1.0f, 1.0f, 1.0f);
	trapObj->AddComponent(trapMod);
	trapObj->AddComponent(trigger);
	trap->AddGameObject(trapObj);
	glm::vec3 Position = player->local.getPosition();
	trap->Scale(scale);
	trap->Translate(Position.x*6, 0, Position.z*6);
	//trap->Rotate(90.0f, glm::vec3(1, 0, 0));
	Collider* col = nullptr;
	for (ComponentSystem::Component* comp : enemy->children[0]->gameObject->components)
	{
		if (comp->GetComponentType() == ComponentSystem::ComponentType::Collider)
			col = (Collider*)comp;
	}
	if (col != nullptr)
		enemyCollider = col;
	return std::move(trap);
}