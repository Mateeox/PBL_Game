#pragma once
#include <map>
#include "MapGenerator/MapKey.hpp"
#include "MapGenerator/MapElement.hpp"

class MapConverter {
	private:
		std::vector<MapElement*>* elements;
		std::map<MapKey*, MapKey::MapType> mapped;
		void CheckWalls(MapElement* element);
		void AddWall(glm::vec2 pos);
	public:
		MapConverter(std::vector<MapElement*>* elements);		
		std::map<MapKey*, MapKey::MapType> Convert();
};