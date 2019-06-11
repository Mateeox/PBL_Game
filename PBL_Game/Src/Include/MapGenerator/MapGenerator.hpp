#pragma once
#include <time.h>
#include "MapConverter.hpp"

class MapGenerator //: public Script
{
	
	private:
	std::vector <MapElement*> maps;
	std::vector<glm::vec2> positions;
	std::vector<MapElement*> doors;
	std::vector<MapKey*> mapped;
	std::vector<SceneNode*>* sNodes;
	Shader* shader;
	Player* player;

	//models
	Model* floor;
	Model* wall;
	Model* door;
	Model* key;
	Model* chest;

	int GetDirection();
	void CheckForWalls();
	void CheckForDoors();
	void ValidateDoors();
	void ClearDoors();
	int CountDoors();
	void SingleDoor();
	void PickDoors();
	void GenerateMap(int n);
	void FinishGeneration();
	bool CheckIfAvailiable(glm::vec2 pos);
	glm::vec2 GetVector2(int step, int boundry, int &parent);
	glm::vec4 GetVector4(glm::vec2 direction);
	MapElement* GetElement(glm::vec2 pos);
	int GetRandomIndex(int max);
	void FillWithNull(int max);
	bool CheckIfNull(int x, int y);
	int FindAnyNeighbour(int boundry, int& move);
	void DefineModels();
	void GenerateChests(int amount);

	public:
	int Squares = 1;
	int Doors = 0;
	int Chests = 0;
	int maxSize;
	bool GlassDoor = false;
	std::vector<SceneNode*> leftnodes;
	std::vector<SceneNode*> rightnodes;
	MapGenerator(Shader* shaderProgram);
	MapGenerator(Shader* shaderProgram, int squares, int doors, int chests, bool glass_door, std::vector<SceneNode*>* sNodes, Player* player);
	void TransformToPositive();
	std::vector<MapKey*> GetConverted();
};