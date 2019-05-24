#include "Component/MapGenerator.hpp"

MapGenerator::MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram)
{
	this->nodes = nodes;
	this->shader = shaderProgram;
	srand(time(NULL));
	GenerateMap(4);
	CheckForWallsNDoors();
	FinishGeneration();
	maps.clear();
	positions.clear();
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
	CheckForWallsNDoors(doors, glass_doors);
	FinishGeneration();
	maps.clear();
	positions.clear();
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

int MapGenerator::GetDirection()
{
	return (rand() % 5);
}

void MapGenerator::CheckForWallsNDoors(int doors, bool glass_door)
{
	for (int i = 0; i < maps.size() - 1; i++)
	{
		MapElement* it = maps[i];
		std::vector<glm::vec2> neighbours = it->GetNeighbours();
		for (int j = 0; j < neighbours.size() - 1; j++)
		{
			if (GetElement(neighbours[j]) == nullptr)
				it->SetWall(GetVector4(it->Position - neighbours[j]));
			else
				it->SetDoor(GetVector4(it->Position - neighbours[j]));
		}
	}
}

void MapGenerator::FinishGeneration()
{
	SceneNode* mapRoot = new SceneNode();
	for (int i = 0; i < maps.size() - 1; i++)
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
			case 0: {
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

void MapGenerator::RandomizeKeysAndDoors()
{
	/*std::vector<glm::vec2> keys;
	for (std::map<glm::vec2, MapElement*, Vec2Comp>::iterator iterator = map.begin(); iterator != map.end(); iterator++)
	{
		keys.push_back(iterator->first);
	}*/
}

MapElement * MapGenerator::GetElement(glm::vec2 pos)
{
	for (int i = 0; i < maps.size() - 1; i++)
	{
		if (maps[i]->Position == pos)
			return maps[i];
	}
	return nullptr;
}

bool MapGenerator::CheckIfAvailiable(glm::vec2 pos)
{
	return std::find(this->positions.begin(), this->positions.end(), pos) == this->positions.end();
}