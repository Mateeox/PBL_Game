#include "Player.hpp"
#include "Triggers/EnemyDies.hpp"
#include "Component/Component.hpp"

Player::Player(SceneNode *aPlayer, int aPartsLimit, Shader &aShader, SceneNode *aNode, SceneNode *enemy)
{
	parentNode = aNode;
	player = aPlayer;
	this->enemy = enemy;
	partsLimit = aPartsLimit;
	PartsAmount = 0;
	trapMod = new Model("Models/Trap/Trap_Anim.fbx", aShader, false);
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

void Player::Update(PBLGame::Window *okienko, float scale)
{
	if (glfwGetKey(okienko->window, GLFW_KEY_SPACE) == GLFW_PRESS && Trap())
	{
		if (!trapSet)
		{
			PartsAmount -= partsLimit;
			parentNode->AddChild(CreateTrap(scale));
			trapSet =true;
		}
	}

	if(enemyTrigger == nullptr)
	{
		std::cout<<"No collider \n";
	}

	if (enemyTrigger != nullptr && trigger != nullptr)
	{
		if (enemyTrigger->checkCollision(trigger))
			trigger->ActivateTrigger();
	}
	
}

SceneNode *Player::CreateTrap(float scale)
{
	SceneNode *trap = new SceneNode();
	GameObject *trapObj = new GameObject(trap->local);
	trapObj->setTag("trap");
	trigger = new EnemyDies(trap->local, enemy); //doda� przeciwnika
	trigger->setDimensions(0, 0, 0, 2.0f, 2.0f, 2.0f);
	trapObj->AddComponent(trapMod);
	trapObj->AddComponent(trigger);
	trap->AddGameObject(trapObj);
	glm::vec3 Position = player->local.getPosition();
	trap->Scale(scale);
	trap->Translate(Position.x * 6, 0, Position.z * 6);
	//trap->Rotate(90.0f, glm::vec3(1, 0, 0));
	Trigger *col = nullptr;
	for (ComponentSystem::Component *comp : enemy->children[0]->gameObject->components)
	{
		if (comp->GetComponentType() == ComponentSystem::ComponentType::Trigger)
			col = (Trigger *)comp;
	}
	if (col != nullptr)
		enemyTrigger = col;
	return std::move(trap);
}