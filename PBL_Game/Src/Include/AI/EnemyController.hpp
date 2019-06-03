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

    //Interest levels
    const float MaxNotInterested = 200;
    const float MaxInterested = 600;
    const float MaxFollowing = 600;
    const float MaxAlwaysFollow = 1000;
    
    float InterestMeter = 0;
    const float InterestMeterIncrement = 2;
    SceneNode *enemy;
    SceneNode *player;

    GridLocation start;
    GridLocation Currenttarget;

   const  GridLocation firstTarget;


    void SetStateFromInterestLevel(); 

public:
    EnemyController(SceneNode * enemy,SceneNode * player,GridLocation aStart,GridLocation aFirstTarget);
    void ChangeEnemyState(EnemyState state);
    float GetPlayerDistance();
    void Update();
};