#pragma once
#include <map>
#include "MapGenerator/MapKey.hpp"
#include "MapGenerator/MapElement.hpp"

class MapConverter {
	private:
		std::vector<MapElement*>* elements;
		std::vector<MapKey*> mapped;
		std::vector<SceneNode*>* nodes;
		void CheckWalls(MapElement* element);
		void AddWall(glm::vec2 pos);
		bool CheckIfExists(glm::vec2 pos);
	public:
		MapConverter(std::vector<MapElement*>* elements, std::vector<SceneNode*>* nodes);
		std::vector<MapKey*> Convert();
};