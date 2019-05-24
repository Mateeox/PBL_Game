#include "Component/MapGenerator.hpp"

MapGenerator::MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram)
{
	this->nodes = nodes;
	this->shader = shaderProgram;
	srand(time(NULL));
	GenerateMap(4);
	CheckForWalls();
	CheckForDoors();
	FinishGeneration();
	maps.clear();
	positions.clear();
	this->doors.clear();
}

MapGenerator::MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram, int squares, int doors, bool glass_doors)
{
	this->Squares = squares;
	this->Doors = doors;
	this->GlassDoor = glass_doors;
	this->nodes = nodes;
	this->shader = shaderProgram;
	srand(time(NULL));
	GenerateMap(squares);
	CheckForWalls();
	CheckForDoors();
	FinishGeneration();
	maps.clear();
	positions.clear();
	this->doors.clear();
}

void MapGenerator::GenerateMap(int n)
{
	MapElement* element = new MapElement(glm::vec2(0, 0), shader);
	maps.push_back(element);
	positions.push_back(element->Position);
	int elementIteration = 0;
	for (int i = 0; i < n-1; i++)
	{
		MapElement* temp = new MapElement(GetVector2(&elementIteration), shader, elementIteration);
		maps.push_back(temp);
		positions.push_back(temp->Position);
		elementIteration = positions.size() - 1;
	}
}

void MapGenerator::CheckForWalls()
{
	for (int i = 0; i < maps.size(); i++)
	{
		MapElement* it = maps[i];
		std::vector<glm::vec2> neighbours = it->GetNeighbours();
		for (int j = 0; j < neighbours.size(); j++)
		{
			if (GetElement(neighbours[j]) == nullptr)
				it->SetWall(GetVector4(it->Position - neighbours[j]));
		}
	}
}

void MapGenerator::CheckForDoors()
{
	for (int i = 0; i < maps.size(); i++)
	{
		MapElement* element = maps[i];
		glm::vec4 walls = element->Walls;
		bool flag_door = false;
		if (walls.x == 1.0f && walls.z == 1.0f) {
			flag_door = true;
			if (walls.y == 0) {
				element->SetDoor(glm::vec4(0, 1.0f, 0, 0));
			}
			if (walls.w == 0) {
				element->SetDoor(glm::vec4(0, 0, 0, 1.0f));
			}
		}

		if(element->Walls.y == 1.0f && element->Walls.w == 1.0f)
		{
			flag_door = true;
			if (walls.x == 0) {
				element->SetDoor(glm::vec4(1.0f, 0, 0, 0));
			}
			if (walls.z == 0) {
				element->SetDoor(glm::vec4(0, 0, 1.0f, 0));
			}
		}
		if (flag_door)
			doors.push_back(element);
	}
	ValidateDoors();
	ClearDoors();
	PickDoors();
}

void MapGenerator::FinishGeneration()
{
	SceneNode* mapRoot = new SceneNode();
	for (int i = 0; i < maps.size(); i++)
	{
		mapRoot->AddChild(maps[i]->GenerateNode(nodes, mapRoot));
	}
	nodes->push_back(mapRoot);
}

glm::vec2 MapGenerator::GetVector2(int* step)
{
	glm::vec2 pos_add;
	while (!CheckIfAvailiable(positions[*step] + pos_add)) {
		int move = GetDirection();
		switch (move)
		{
			default: {
				*step = maps[*step]->ParentElement;
				break;
			}
			case 1: {
				pos_add = glm::vec2(0, 1.0f);
				break;
			}
			case 2: {
				pos_add = glm::vec2(1.0f, 0);
				break;
			}
			case 3: {
				pos_add = glm::vec2(0, -1.0f);
				break;
			}
			case 4: {
				pos_add = glm::vec2(-1.0f, 0);
				break;
			}
		}
	}
	return pos_add + this->positions[*step];
}

glm::vec4 MapGenerator::GetVector4(glm::vec2 direction)
{
	switch ((int)direction.x)
	{
		case 1:
		{
			return glm::vec4(0, 0, 0, 1.0f);
		}
		case -1:
		{
			return  glm::vec4(0, 1.0f, 0, 0);
		}
	}

	switch ((int)direction.y)
	{
		case 1:
		{
			return glm::vec4(0, 0, 1.0f, 0);
		}
		case -1:
		{
			return glm::vec4(1.0f, 0, 0, 0);
		}
	}
}

MapElement * MapGenerator::GetElement(glm::vec2 pos)
{
	for (int i = 0; i < maps.size(); i++)
	{
		if (maps[i]->Position == pos)
			return maps[i];
	}
	return nullptr;
}

bool MapGenerator::CheckIfAvailiable(glm::vec2 pos)
{
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i] == pos)
			return false;
	}
	return true;
}

void MapGenerator::ValidateDoors()
{
	for (std::vector<MapElement*>::iterator door = doors.begin(); door != doors.end(); door++)
	{
		MapElement* element = *door;
		if (element->DoesHaveADoor()) {
			std::vector<glm::vec2> neighbours = element->GetNeighbours();
			for (int j = 0; j < neighbours.size(); j++)
			{
				for (std::vector<MapElement*>::iterator it = doors.begin(); it != doors.end(); it++)
				{
					MapElement* neighbour = *it;
					if (neighbour->Position == neighbours[j] && neighbour->DoesHaveADoor())
						neighbour->RemoveDoor(element->Doors);
				}
			}
		}
	}
}

void MapGenerator::ClearDoors()
{
	std::vector<MapElement*>::iterator it = doors.begin();
	do
	{
		MapElement* temp = *it;
		if (!temp->DoesHaveADoor())
			it = doors.erase(it);
		else
			it++;
	} while (it != doors.end());
}

int MapGenerator::CountDoors()
{
	int count = 0;
	for (std::vector<MapElement*>::iterator door = doors.begin(); door != doors.end(); door++)
	{
		MapElement* el = *door;
		count += el->CountDoors();
	}
	return count;
}

void MapGenerator::PickDoors()
{
	int doorsCount = CountDoors();
	do
	{
		int index = GetRandomIndex(doors.size());
		int count = doors[index]->CountDoors();
		if (doorsCount - count >= Doors) {
			doors[index]->CleanDoors();
			doorsCount -= count;
		}
		else {
			doors[index]->RemoveDoor(GetRandomIndex(1));
			doorsCount--;
		}
	} while (Doors != doorsCount);
}

int MapGenerator::GetDirection()
{
	return (rand() % 6);
}

int MapGenerator::GetRandomIndex(int max)
{
	return (rand() % max);
}