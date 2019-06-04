#include "EnemyController.hpp"
#include "PathFinding/PathFindingUtils.hpp"
#include "../PathFinding/MapTileRenderUtils.hpp"


EnemyController::EnemyController(SceneNode &aEnemy,
	SceneNode &aPlayer,
	GridLocation aStart,
	GridLocation aFirstTarget,
	GridWithWeights& aGrid,
	std::vector<MapTile *>&aMapTiles,
	int aMapSize,
	std::unordered_map<GridLocation, GridLocation>& aCame_from,
	std::unordered_map<GridLocation, double>& aCost_so_far) : enemy(aEnemy),
	player(aPlayer),
	start(aStart),
	firstStart(aStart),
	firstTarget(aFirstTarget),
	Currenttarget(aFirstTarget),
	grid(aGrid),
	mapTiles(aMapTiles),
	came_from(aCame_from),
	cost_so_far(aCost_so_far),
	mapSize(aMapSize)
{


}

void EnemyController::ChangeEnemyState(EnemyState aState)
{
    state = aState;
}
float EnemyController::GetPlayerDistance()
{

    int x1 = enemy.local.getPosition().x * enemy.local.getScale().x;
    int x2 = player.local.getPosition().x * player.local.getScale().x;

    int z1 = enemy.local.getPosition().z * enemy.local.getScale().z;
    int z2 = player.local.getPosition().z * player.local.getScale().z;

    return sqrt((x1 - x2) * (x1 - x2) + (z1 - z2) * (z1 - z2)) * 10;
}
void EnemyController::Update(float  interpolation)
{
	EnemyPlayerDistance = GetPlayerDistance();
    if (EnemyPlayerDistance < InterestDistance && InterestMeter < MaxAlwaysFollow)
    {
        if (EnemyPlayerDistance > minPlayerDistance && EnemyPlayerDistance != 0)
        {
            InterestMeter += InterestMeterIncrement * (1 / (EnemyPlayerDistance * DistanceToInterestRatio));
        }
        else
        {
            InterestMeter = MaxAlwaysFollow;
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
    CheckIFNotOnEnd();

	if (debugPathFinding)
		ResetMapTilePath(mapTiles, grid, mapSize, &path);

	if (path.size() > 1)
		MoveNodeToMapTile(&enemy, path[1], interpolation, 10, 3, 5); // TODO Add BaseSpeed

	start = GetPositionOfset(enemy, mapSize, 3, 5);

	if (grid.passable(Currenttarget))
	{
		a_star_search(grid, start, Currenttarget, came_from, cost_so_far);
		path = reconstruct_path(start, Currenttarget, came_from);
	}

    //UpdatePosition
}

void EnemyController::CheckIFNotOnEnd()
{
	if(path.size() == 1)
	SwtichStartWithEnd();
	
	if (grid.passable(Currenttarget))
	{
		a_star_search(grid, start, Currenttarget, came_from, cost_so_far);
		path = reconstruct_path(start, Currenttarget, came_from);
	}
	LastFirstFlag != LastFirstFlag;
}

void EnemyController::SwtichStartWithEnd()
{
    if (LastFirstFlag)
    {
        Currenttarget = firstStart;
    }
    else
    {
        Currenttarget = firstTarget;
    }
	
}

void EnemyController::SetTarget()
{
    switch (state)
    {
    case NotInteresed:
        Currenttarget = firstTarget;
        break;

    case Interested:
        //Stop enemy
        break;

    case Following:
        // find girdlocation and set as currectTarget
        break;

    case AlwaysFollow:
        //find girdlocation and set as currentTarget
        break;
    }
}
void EnemyController::SetStateFromInterestLevel()
{
    if (InterestMeter < MaxAlwaysFollow)
    {
        if (InterestMeter >= 0 && InterestMeter < MaxNotInterested)
        {
            state = NotInteresed;
        }
        else if (InterestMeter >= MaxNotInterested && InterestMeter < MaxInterested)
        {
            state = Interested;
        }
        else if (InterestMeter >= MaxInterested && InterestMeter < MaxFollowing)
        {
            state = Following;
        }
    }
    else
    {
        state = AlwaysFollow;
    }
}

void EnemyController::MoveNodeToMapTile(SceneNode *sceneNode, GridLocation mapTile, float interpolation, float speed, float NodeXOffset, float NodeZOffset)
{
	glm::vec2 positionA{ sceneNode->local.getPosition().x, sceneNode->local.getPosition().z };
	glm::vec2 positionB{ NodeXOffset + mapTile.x * 400, NodeZOffset + mapTile.y * 400 };

	glm::vec2 diffVec = positionB - positionA;

	glm::vec3 diffVec3D = { diffVec.x, sceneNode->local.getPosition().y, diffVec.y };

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
	if (abs(diffVec.y) > abs(diffVec.x))
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
}