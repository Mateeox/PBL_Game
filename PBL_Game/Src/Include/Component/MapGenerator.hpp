#pragma once
#include "MapElement.hpp"
#include <map>
#include <time.h>

class MapGenerator //: public Script
{
	private:
	std::map <glm::vec2, MapElement> map;
	
	float GetDirection();

	public:
	MapGenerator();
	MapGenerator(int squares, int doors, bool glass_door);
	void GenerateMap(int n);
};