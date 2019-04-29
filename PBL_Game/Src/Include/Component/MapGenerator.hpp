#pragma once
#include "MapElement.hpp"
#include <map>
#include <time.h>

class MapGenerator //: public Script
{
	private:
	std::map <glm::vec2, MapElement> map;
	std::vector<SceneNode>* nodes;

	float GetDirection(bool canBeZero);
	void CheckForWallsNDoors();
	void GenerateMap(int n);
	void FnishGeneration();

	public:
	MapGenerator(std::vector<SceneNode>* nodes);
	MapGenerator(std::vector<SceneNode>* nodes, int squares, int doors, bool glass_door);
};