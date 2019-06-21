#include "Player.hpp"
#include "Triggers/TrapTrigger.hpp"
#include "Component/Component.hpp"
#include "Game.hpp"

Player::Player(SceneNode *aLeftPlayer,
			   SceneNode *aRightPlayer,
			   int aPartsLimit,
			   Shader &aShader,
			   SceneNode *aLeftScene,
			   SceneNode *aRightScene,
			   SceneNode *aEnemy,
			   SimpleGUI::GuiElement *aBackground,
			   SimpleGUI::GuiElement *aWin, Game *aGame) : game(aGame),
														   background(aBackground),
														   win(aWin),
														   leftPlayer(aLeftPlayer),
														   rightPlayer(aRightPlayer),
														   leftScene(aLeftScene),
														   rightScene(aRightScene),
														   enemy(aEnemy),
														   partsLimit(aPartsLimit),
														   PartsAmount(0),
														   trigger(nullptr),
														   trapMod(new Model("Models/Trap/Trap_Anim.fbx", aShader, false)) {}

int Player::Parts()
{
	return PartsAmount;
}

bool Player::Trap()
{
	return PartsAmount == partsLimit;
}

void Player::AddTrapPart()
{
	if (!isAdding)
	{
		isAdding = false;
		PartsAmount++;

		switch (PartsAmount)
		{
		case 1:
			game->TrapPartInfo->SwitchTexture("Parts1");
			break;
		case 2:
			game->TrapPartInfo->SwitchTexture("Parts2");
			break;
		case 3:
			game->TrapPartInfo->SwitchTexture("Parts3");
			break;
		case 4:
			game->TrapPartInfo->SwitchTexture("Parts4");
			break;
		default:
			game->TrapPartInfo->SwitchTexture("default");
			break;
		}
	}
}

void Player::Update(PBLGame::Window *okienko, float scale)
{
	if (glfwGetKey(okienko->window, GLFW_KEY_SPACE) == GLFW_PRESS && Trap())
	{
		if (!trapSet)
		{
			PartsAmount = 0;

			if (game->leftSideActive)
			{
				leftScene->AddChild(CreateTrap(scale));
			}
			else
			{
				rightScene->AddChild(CreateTrap(scale));
			}
			game->TrapPartInfo->SwitchTexture("default");
			trapSet = true;
		}
	}

	if (enemyTrigger != nullptr && trigger != nullptr)
	{
		if (enemyTrigger->checkCollision(trigger))
		{
			trigger->ActivateTrigger();
		}
	}
}

SceneNode *Player::CreateTrap(float scale)
{
	glm::vec3 Position;

	SceneNode *trap = new SceneNode();
	GameObject *trapObj = new GameObject(trap->local);
	trapObj->setTag("trap");
	trigger = new TrapTrigger(trap->local, enemy, background, win); //doda� przeciwnika
	trigger->setDimensions(0, 0, 0, 1.0f, 1.0f, 1.0f);
	trapObj->AddComponent(trapMod);
	trapObj->AddComponent(trigger);
	trap->AddGameObject(trapObj);

	if (game->leftSideActive)
	{
		Position = leftPlayer->local.getPosition();
	}
	else
	{
		Position = rightPlayer->local.getPosition();
	}

	trap->Scale(scale);
	trap->Translate(Position.x * 6, 0, Position.z * 6);
	//trap->Rotate(90.0f, glm::vec3(1, 0, 0));
	EnemyTrigger *col = nullptr;
	for (ComponentSystem::Component *comp : enemy->children[0]->gameObject->components)
	{
		if (comp->GetComponentType() == ComponentSystem::ComponentType::Trigger)
			col = (EnemyTrigger *)comp;
	}
	if (col != nullptr)
		enemyTrigger = col;
	return std::move(trap);
}