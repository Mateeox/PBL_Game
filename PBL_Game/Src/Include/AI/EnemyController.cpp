#include "EnemyController.hpp"

EnemyController::EnemyController(SceneNode &aEnemy,
	SceneNode &aPlayer,
	GridLocation aStart,
	GridLocation aFirstTarget,
	GridWithWeights& aGrid) : enemy(aEnemy),
	player(aPlayer),
	start(aStart),
	firstStart(aStart),
	firstTarget(aFirstTarget),
	Currenttarget(aFirstTarget),
	grid(aGrid)
{


}

void EnemyController::ChangeEnemyState(EnemyState aState)
{
    state = aState;
}
float EnemyController::GetPlayerDistance()
{

    int x1 = enemy.local.getPosition().x*enemy.local.getScale().x;
    int x2 = player.local.getPosition().x*player.local.getScale().x;

    int z1 = enemy.local.getPosition().z*enemy.local.getScale().z;
    int z2 = player.local.getPosition().z*player.local.getScale().z;

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
            InterestMeter -= InterestMeterIncrement;
			if (InterestMeter < 0)
			{
				InterestMeter = 0;
			}
        }
    }
    SetStateFromInterestLevel();
    SetTarget();
    CheckIFNotOnEnd();

    //UpdatePosition
}

void EnemyController::CheckIFNotOnEnd()
{
    //check if currnt node position != firstTarget , if it is switch target with startposition // sometings is not yes
}

void EnemyController::SwtichStartWithEnd()
{
    if (LastFirstFlag)
    {
        Currenttarget = firstStart;
        LastFirstFlag != LastFirstFlag;
    }
    else
    {
        Currenttarget = firstTarget;
        LastFirstFlag != LastFirstFlag;
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