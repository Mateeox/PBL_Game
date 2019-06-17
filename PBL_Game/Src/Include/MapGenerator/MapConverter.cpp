#include "MapConverter.hpp"

MapConverter::MapConverter(std::vector<MapElement*>* elements, std::vector<SceneNode*>* nodes)
{
	this->elements = elements;
	this->nodes = nodes;
}

std::vector<MapKey*> MapConverter::Convert()
{
	for (int i = 0; i < elements->size(); i++)
	{
		MapElement* element = elements->at(i);
		MapType type = MapType::Null;

		//Check what type of this element is

		if (element->DoesHaveADoor())
			type = MapType::Blocked;
		else
			type = MapType::Floor;


		//Adding walls if not exist yet
		
		CheckWalls(element);
		MapKey* key = new MapKey(element->Position.x, element->Position.y, type);
		mapped.push_back(key);
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
	if (!CheckIfExists(pos)) {
		MapKey* key = new MapKey(pos.x, pos.y, MapType::Wall);
		mapped.push_back(key);

		SceneNode* wall = new SceneNode();
		GameObject* wallObj = new GameObject(wall->local);
		wallObj->setTag("Roof");

		wall->AddGameObject(wallObj);
		wall->Translate(pos.x, 0, pos.y);

		wall->Rotate(90.0f, glm::vec3(1, 0, 0));
		wall->Scale(0.0255f, 0.0255f, 0.01f);
		nodes->push_back(wall);
	}
}

bool MapConverter::CheckIfExists(glm::vec2 pos)
{
	for (int i = 0; i < mapped.size(); i++)
	{
		if (pos.x == mapped.at(i)->x && pos.y == mapped.at(i)->y)
			return true;
	}
	return false;
}