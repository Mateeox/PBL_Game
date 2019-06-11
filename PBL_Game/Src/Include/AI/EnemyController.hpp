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
    const float DistanceToInterestRatio = 0.65;

	int mapSize;

    //Interest levels
	float MinNotInterested;
	float MinInterested;
	float MinFollowing;
	float MinAlwaysFollow;
	float enemySpeed;
	float enemyRunSpeed;
	float enemyWalkSpeed;

    
    const float InterestMeterIncrement = 100;
    SceneNode &enemy;
    SceneNode &player;
	GridWithWeights& grid;
	std::unordered_map<GridLocation, GridLocation> came_from;
	std::unordered_map<GridLocation, double> cost_so_far;
	std::vector<GridLocation> path;
	std::vector<MapTile *>& mapTiles;

	bool debugPathFinding = true;
	bool StopEnemy = false;
    bool LastFirstFlag = true;

    const GridLocation firstTarget;
    const GridLocation firstStart;

    void SetStateFromInterestLevel();
    void SetTarget();
    void CheckIFNotOnEnd();

public:
    EnemyController(SceneNode &enemy,
		            SceneNode &player,
		            GridLocation aStart,
		            GridLocation aFirstTarget,
		            GridWithWeights& grid,
		            std::vector<MapTile *>&mapTiles,
					int mapSize);

    void ChangeEnemyState(EnemyState state);
    float GetPlayerDistance();
    void Update(float interpolation);
    void SwtichStartWithEnd();
	void MoveEnemyToMapTile(SceneNode *enemyNode, GridLocation mapTile, float interpolation, float speed, float NodeXOffset, float NodeZOffset);
	void MoveEnemyToNode(SceneNode *enemyNode, SceneNode * targetNode, float interpolation, float speed);
    void LoadFromConfig();

	float EnemyPlayerDistance;
	float InterestMeter = 0;
    GridLocation start;
    GridLocation Currenttarget;
};