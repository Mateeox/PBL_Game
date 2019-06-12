#include "EnemyController.hpp"
#include "PathFinding/PathFindingUtils.hpp"
#include "../PathFinding/MapTileRenderUtils.hpp"
#include "Configuration/ConfigUtils.hpp"


EnemyController::EnemyController(Game * aGame,SceneNode &aEnemy,
								 SceneNode &aLPlayer,
								 SceneNode &aRPlayer,
								 GridLocation aStart,
								 GridLocation aFirstTarget,
								 GridWithWeights &aGrid,
								 std::vector<MapTile *> &aMapTiles,
								 int aMapSize) : 
								 				 game(aGame),
												 enemy(aEnemy),
												 currentPlayer(&aLPlayer),
												 leftplayer(aLPlayer),
												 rightplayer(aRPlayer),
												 start(aStart),
												 firstStart(aStart),
												 firstTarget(aFirstTarget),
												 Currenttarget(aFirstTarget),
												 grid(aGrid),
												 mapTiles(aMapTiles),
												 mapSize(aMapSize)
{

	LoadFromConfig();
}

void EnemyController::LoadFromConfig()
{
	using namespace ConfigUtils;
	MinNotInterested = GetValueFromMap<float>("MinNotInterested", GlobalConfigMap);
	MinInterested = GetValueFromMap<float>("MinInterested", GlobalConfigMap);
	MinFollowing = GetValueFromMap<float>("MinFollowing", GlobalConfigMap);
	MinAlwaysFollow = GetValueFromMap<float>("MinAlwaysFollow", GlobalConfigMap);
	enemySpeed = GetValueFromMap<float>("EnemyBaseSpeed", GlobalConfigMap);
	enemyRunSpeed = GetValueFromMap<float>("EnemyRunSpeed", GlobalConfigMap);
	enemyWalkSpeed = GetValueFromMap<float>("EnemyBaseSpeed", GlobalConfigMap);
}

void EnemyController::ChangeEnemyState(EnemyState aState)
{
	state = aState;
}
float EnemyController::GetPlayerDistance()
{

	int x1 = enemy.local.getPosition().x * enemy.local.getScale().x;
	int x2 = currentPlayer->local.getPosition().x * currentPlayer->local.getScale().x;

	int z1 = enemy.local.getPosition().z * enemy.local.getScale().z;
	int z2 = currentPlayer->local.getPosition().z * currentPlayer->local.getScale().z;

	return sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2)) * 10;
}

void EnemyController::SwtichPlayer()
{
	if (currentPlayer == &leftplayer)
	{
		currentPlayer = &rightplayer;
	}
	else
	{
		currentPlayer = &leftplayer;
	}
}
void EnemyController::Update(float interpolation)
{

	EnemyPlayerDistance = GetPlayerDistance();
	if (EnemyPlayerDistance < InterestDistance && InterestMeter < MinAlwaysFollow)
	{
		if (EnemyPlayerDistance > minPlayerDistance && EnemyPlayerDistance != 0)
		{
			InterestMeter += InterestMeterIncrement * (1 / (EnemyPlayerDistance * DistanceToInterestRatio));
		}
		else
		{
			InterestMeter = MinAlwaysFollow;
		}
	}
	else
	{
		if (state != AlwaysFollow && InterestMeter > 0)
		{
			InterestMeter -= InterestMeterIncrement * (1 / (EnemyPlayerDistance * DistanceToInterestRatio));
			if (InterestMeter < 0)
			{
				InterestMeter = 0;
			}
		}
	}
	SetStateFromInterestLevel();
	SetTarget();

	if (debugPathFinding)
		ResetMapTilePath(mapTiles, grid, mapSize, &path);

	if (!StopEnemy)
	{
		if (EnemyPlayerDistance > 25)
		{
			if (path.size() > 1)
			{
				MoveEnemyToMapTile(&enemy, path[1], interpolation, enemySpeed, 3, 5); // TODO Add BaseSpeed
			}
		}
		else
		{
			MoveEnemyToNode(&enemy, currentPlayer, interpolation, enemySpeed);
		}
	}

	start = GetPositionOfset(enemy, mapSize, 3, 5);
	CheckIFNotOnEnd();

	if (grid.passable(Currenttarget))
	{
		a_star_search(grid, start, Currenttarget, came_from, cost_so_far);
		path = reconstruct_path(start, Currenttarget, came_from);
	}
}

void EnemyController::CheckIFNotOnEnd()
{
	if (path.size() == 1)
	{
		SwtichStartWithEnd();
		if (grid.passable(Currenttarget))
		{
			a_star_search(grid, start, Currenttarget, came_from, cost_so_far);
			path = reconstruct_path(start, Currenttarget, came_from);
		}
		LastFirstFlag != LastFirstFlag;
	}
}

void EnemyController::SwtichStartWithEnd()
{
	
	if (Currenttarget == firstStart)
	{
		Currenttarget = firstTarget;
		if(GetPlayerDistance() <25)
		{
		game->EnemyOnLefSide = !game->EnemyOnLefSide;
		SwtichPlayer();
		}
	}
	else
	{
		Currenttarget = firstStart;
		if(GetPlayerDistance() <25)
		{
		game->EnemyOnLefSide = !game->EnemyOnLefSide ;
		SwtichPlayer();
		}
	}

	
}

void EnemyController::SetTarget()
{
	switch (state)
	{
	case NotInteresed:
		StopEnemy = false;
		break;

	case Interested:
		Currenttarget = GetPositionOfset(*currentPlayer, mapSize, 7, 5);
		StopEnemy = true;
		break;

	case Following:
		Currenttarget = GetPositionOfset(*currentPlayer, mapSize, 7, 5);
		StopEnemy = false;
		enemySpeed = enemyWalkSpeed;
		break;

	case AlwaysFollow:
		Currenttarget = GetPositionOfset(*currentPlayer, mapSize, 7, 5);
		StopEnemy = false;
		enemySpeed = enemyRunSpeed;
		break;
	}
}
void EnemyController::SetStateFromInterestLevel()
{
	if (InterestMeter < MinAlwaysFollow)
	{
		if (InterestMeter >= 0 && InterestMeter < MinNotInterested)
		{
			state = NotInteresed;
		}
		else if (InterestMeter >= MinInterested && InterestMeter < MinFollowing)
		{
			state = Interested;
		}
		else if (InterestMeter >= MinFollowing && InterestMeter < MinAlwaysFollow)
		{
			state = Following;
		}
	}
	else
	{
		state = AlwaysFollow;
	}
}

void EnemyController::MoveEnemyToMapTile(SceneNode *sceneNode, GridLocation mapTile, float interpolation, float speed, float NodeXOffset, float NodeZOffset)
{
	glm::vec2 positionA{sceneNode->local.getPosition().x, sceneNode->local.getPosition().z};
	glm::vec2 positionB{NodeXOffset + mapTile.x * 400, NodeZOffset + mapTile.y * 400};

	glm::vec2 diffVec = positionB - positionA;

	glm::vec3 diffVec3D = {diffVec.x, sceneNode->local.getPosition().y, diffVec.y};

	double veclenght = sqrt(diffVec.x * diffVec.x + diffVec.y * diffVec.y);
	float angle = atan2f(diffVec3D.y, diffVec3D.x) * 180.0f / 3.14f;

	if (veclenght != 0)
	{
		diffVec.x = diffVec.x / veclenght;
		diffVec.y = diffVec.y / veclenght;
	}
	else
	{
		diffVec = glm::vec2(0, 0);
	}

	float value = interpolation * speed;
	diffVec *= value;

	SceneNode *roationChild = sceneNode->children[0];
	sceneNode->Translate(diffVec.x, 0, diffVec.y);

	float playerRotation = atan2(-diffVec.y, diffVec.x);
	roationChild->local.SetRotation(0, playerRotation * 58 + 90, 0);

	/*if (abs(diffVec.y) > abs(diffVec.x))
	{
		if (diffVec.y < 0)
		{
			roationChild->local.SetRotation(0, 180, 0);
		}
		else
		{
			roationChild->local.SetRotation(0, 0, 0);
		}
	}
	else
	{
		if (diffVec.x < 0)
		{
			roationChild->local.SetRotation(0, 270, 0);
		}
		else
		{
			roationChild->local.SetRotation(0, 90, 0);
		}
	}
	*/
}

void EnemyController::MoveEnemyToNode(SceneNode *sceneNode, SceneNode *targetNode, float interpolation, float speed)
{
	glm::vec2 positionA{sceneNode->local.getPosition().x * sceneNode->local.getScale().x, sceneNode->local.getPosition().z * sceneNode->local.getScale().z};
	glm::vec2 positionB{targetNode->local.getPosition().x * targetNode->local.getScale().x, targetNode->local.getPosition().z * targetNode->local.getScale().z};

	glm::vec2 diffVec = positionB - positionA;
	glm::vec2 diffNormalized;

	glm::vec3 diffVec3D = {diffVec.x, sceneNode->local.getPosition().y, diffVec.y};

	double veclenght = sqrt(diffVec.x * diffVec.x + diffVec.y * diffVec.y);
	float angle = atan2f(diffVec3D.y, diffVec3D.x) * 180.0f / 3.14f;

	if (veclenght != 0)
	{
		diffNormalized.x = diffVec.x / veclenght;
		diffNormalized.y = diffVec.y / veclenght;
	}
	else
	{
		diffNormalized = glm::vec2(0, 0);
	}

	float value = interpolation * speed;
	diffNormalized *= value;

	SceneNode *roationChild = sceneNode->children[0];
	sceneNode->Translate(diffNormalized.x, 0, diffNormalized.y);

	float playerRotation = atan2(-diffNormalized.y, diffNormalized.x);
	roationChild->local.SetRotation(0, playerRotation * 58 + 90, 0);
}