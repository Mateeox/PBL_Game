#pragma once
#include <map>
#include "MapGenerator/MapKey.hpp"
#include "MapGenerator/MapElement.hpp"

class MapConverter {
	private:
		std::vector<MapElement*>::iterator begin;
		std::vector<MapElement*>::iterator end;
		std::map<MapKey, MapType> mapped;
		void CheckWalls(MapElement* element);
		void AddWall(glm::vec2 pos);
	public:
		MapConverter(std::vector<MapElement*>::iterator begin, std::vector<MapElement*>::iterator end);
		std::map<MapKey, MapType> Convert();
};