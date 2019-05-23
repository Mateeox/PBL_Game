#pragma once
#include "MapElement.hpp"
#include <map>
#include <time.h>

class MapGenerator //: public Script
{
	private:
	std::vector <MapElement*> maps;
	std::vector<glm::vec2> positions;
	std::vector<SceneNode*>* nodes;
	Shader* shader;

	int GetDirection();
	void CheckForWallsNDoors(int doors = 0, bool glass_door = false);
	void GenerateMap(int n);
	void FinishGeneration();
	bool CheckIfAvailiable(glm::vec2 pos);
	glm::vec2 GetVector2(int* step);
	glm::vec4 GetVector4(glm::vec2 direction);
	void RandomizeKeysAndDoors();
	MapElement* GetElement(glm::vec2 pos);

	public:
	int Squares = 1;
	int Doors = 0;
	bool GlassDoor = false;
	MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram);
	MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram, int squares, int doors, bool glass_door);
};