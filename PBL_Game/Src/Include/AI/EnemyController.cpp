#include "EnemyController.hpp"

EnemyController::EnemyController(SceneNode *aEnemy,
                                 SceneNode *aPlayer,
                                 GridLocation aStart,
                                 GridLocation aFirstTarget) : enemy(aEnemy),
                                                              player(aPlayer),
                                                              start(aStart),
                                                              firstStart(aStart),
                                                              firstTarget(aFirstTarget),
                                                              Currenttarget(aFirstTarget)
{
}

void EnemyController::ChangeEnemyState(EnemyState aState)
{
    state = aState;
}
float EnemyController::GetPlayerDistance()
{

    int x1 = enemy->local.getPosition().x;
    int x2 = player->local.getPosition().x;

    int y1 = enemy->local.getPosition().y;
    int y2 = player->local.getPosition().y;

    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void EnemyController::Update()
{
    float playerDistance = GetPlayerDistance();
    if (playerDistance > InterestDistance && InterestMeter < MaxAlwaysFollow)
    {
        if (playerDistance > minPlayerDistance && playerDistance != 0)
        {
            InterestMeter += InterestMeterIncrement * (1 / (playerDistance * DistanceToInterestRatio));
        }
        else
        {
            InterestMeter = MaxAlwaysFollow;
        }
    }
    else
    {
        if (state != AlwaysFollow && InterestMeter <= 0)
        {
            InterestMeter -= InterestMeterIncrement;
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