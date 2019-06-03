#include "EnemyController.hpp"

EnemyController::EnemyController(SceneNode *aEnemy,
                                 SceneNode *aPlayer,
                                 GridLocation aStart,
                                 GridLocation aFirstTarget) : enemy(aEnemy),
                                                              player(aPlayer),
                                                              start(aStart),
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

    if(GetPlayerDistance()>InterestDistance)
    {
        InterestMeter+=InterestMeterIncrement;
    }



}