#include <SceneNode.hpp>
#include <PathFinding/PathFindingUtils.hpp>

enum EnemyState
{
    NotInteresed, // 0-200
    Interested,   //200-600
    Following,    //600-1000
    AlwaysFollow  //>1000

};

class EnemyController
{
    EnemyState state = NotInteresed;
    const float InterestDistance = 50;

    const float minPlayerDistance = 0.005f;
    const float DistanceToInterestRatio = 0.5;

    //Interest levels
    const float MaxNotInterested = 200;
    const float MaxInterested = 600;
    const float MaxFollowing = 600;
    const float MaxAlwaysFollow = 1000;

    float InterestMeter = 0;
    const float InterestMeterIncrement = 2;
    SceneNode *enemy;
    SceneNode *player;


    bool LastFirstFlag = true;

    const GridLocation firstTarget;
    const GridLocation firstStart;

    void SetStateFromInterestLevel();
    void SetTarget();
    void CheckIFNotOnEnd();

public:
    EnemyController(SceneNode *enemy, SceneNode *player, GridLocation aStart, GridLocation aFirstTarget);
    void ChangeEnemyState(EnemyState state);
    float GetPlayerDistance();
    void Update();
    void SwtichStartWithEnd();

    GridLocation start;
    GridLocation Currenttarget;
};