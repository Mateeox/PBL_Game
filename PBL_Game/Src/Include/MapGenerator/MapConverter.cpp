#include "MapConverter.hpp"

MapConverter::MapConverter(std::vector<MapElement*>* elements)
{
	this->elements = elements;
}

std::map<MapKey*, MapKey::MapType> MapConverter::Convert()
{
	for (std::vector<MapElement*>::iterator it = elements->begin(); it != elements->end(); it++)
	{
		MapElement* element = *it;
		MapKey* key = new MapKey(element->Position.x, element->Position.y);
		MapKey::MapType type;

		//Check what type of this element is

		if (element->DoesHaveADoor())
			type = MapKey::MapKey::Blocked;
		else
			type = MapKey::MapKey::Floor;

		//Adding walls if not exist yet
		
		CheckWalls(element);

		mapped.insert(std::pair<MapKey*, MapKey::MapType>(key, type));
	}
	return mapped;
}

void MapConverter::CheckWalls(MapElement* element)
{
	glm::vec4 walls = element->Walls;
	//Check if any wall exist
	if (walls != glm::vec4())
	{
		if (walls.x == 1.0f)
			AddWall(element->Position + glm::vec2(0, 1.0f));
		if (walls.y == 1.0f)
			AddWall(element->Position + glm::vec2(1.0f, 0));
		if (walls.z == 1.0f)
			AddWall(element->Position + glm::vec2(0, -1.0f));
		if (walls.w == 1.0f)
			AddWall(element->Position + glm::vec2(-1.0f, 0));
	}
}

void MapConverter::AddWall(glm::vec2 pos)
{
	MapKey* key = new MapKey(pos.x, pos.y);
	if (mapped.find(key) == mapped.end())
		mapped.insert(std::pair<MapKey*, MapKey::MapType>(key, MapKey::MapType::Wall));
}