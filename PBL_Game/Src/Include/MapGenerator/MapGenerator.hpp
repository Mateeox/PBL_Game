#pragma once
#include <time.h>
#include "MapConverter.hpp"

class MapGenerator //: public Script
{
	
	private:
	std::vector <MapElement*> maps;
	std::vector<glm::vec2> positions;
	std::vector<MapElement*> doors;
	std::map<MapKey, MapType> mapped;
	Shader* shader;

	//models
	Model* floor;
	Model* wall;
	Model* door;

	int GetDirection();
	void CheckForWalls();
	void CheckForDoors();
	void ValidateDoors();
	void ClearDoors();
	int CountDoors();
	void PickDoors();
	void GenerateMap(int n);
	void FinishGeneration();
	bool CheckIfAvailiable(glm::vec2 pos);
	glm::vec2 GetVector2(int step);
	glm::vec4 GetVector4(glm::vec2 direction);
	MapElement* GetElement(glm::vec2 pos);
	int GetRandomIndex(int max);
	void FillWithNull(int max );

	public:
	int Squares = 1;
	int Doors = 0;
	int maxSize;
	bool GlassDoor = false;
	std::vector<SceneNode*> nodes;
	MapGenerator(Shader* shaderProgram);
	MapGenerator(Shader* shaderProgram, int squares, int doors, bool glass_door);
	void TransformToPositive();
	std::map<MapKey, MapType> GetConverted();
};