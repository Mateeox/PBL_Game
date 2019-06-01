#include "MapGenerator.hpp"

MapGenerator::MapGenerator(Shader *shaderProgram)
{
	this->shader = shaderProgram;
	floor = new Model("Models/House/StaticNormal_Floor.obj", *shader, false);
	wall = new Model("Models/House/StaticSimpleDestroyedWall.obj", *shader, false);
	door = new Model("Models/House/StaticDoor.obj", *shader, false);
	srand(time(NULL));
	GenerateMap(4);
	CheckForWalls();
	CheckForDoors();
	FinishGeneration();
	MapConverter conv = MapConverter(&maps);
	mapped = conv.Convert();
	positions.clear();
	this->doors.clear();
}

MapGenerator::MapGenerator(Shader *shaderProgram, int squares, int doors, bool glass_doors)
{
	this->Squares = squares;
	this->Doors = doors;
	this->GlassDoor = glass_doors;
	this->shader = shaderProgram;
	floor = new Model("Models/House/StaticNormal_Floor.obj", *shader, false);
	wall = new Model("Models/House/StaticSimpleDestroyedWall.obj", *shader, false);
	door = new Model("Models/House/StaticDoor.obj", *shader, false);
	srand(time(NULL));
	GenerateMap(squares);
	CheckForWalls();
	CheckForDoors();

	TransformToPositive();
	FinishGeneration();
	MapConverter conv = MapConverter(&maps);

	mapped = conv.Convert();

	int max = 0;
	for (MapKey* element : mapped)
	{
		if (element->x > max)
			max = element->x;
		if (element->y > max)
			max = element->y;
	}
	maxSize = max+1;// ewentualna sciana na koncu
	FillWithNull(max);

	std::cout << "Mapped size: " << mapped.size() << "   max: " << max << "\n";
	positions.clear();
	this->doors.clear();
}

std::vector<MapKey*> MapGenerator::GetConverted()
{
	return mapped;
}

void MapGenerator::GenerateMap(int n)
{
	MapElement *element = new MapElement(glm::vec2(0, 0), nodes);
	maps.push_back(element);
	positions.push_back(element->Position);
	int elementIteration = 0;
	for (int i = 0; i < n - 1; i++)
	{
		MapElement *temp = new MapElement(GetVector2(elementIteration), nodes, elementIteration);
		maps.push_back(temp);
		positions.push_back(temp->Position);
		elementIteration = positions.size() - 1;
	}
}

void MapGenerator::CheckForWalls()
{
	for (int i = 0; i < maps.size(); i++)
	{
		MapElement *it = maps[i];
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
		MapElement *element = maps[i];
		glm::vec4 walls = element->Walls;
		bool flag_door = false;
		if (walls.x == 1.0f && walls.z == 1.0f)
		{
			flag_door = true;
			if (walls.y == 0)
			{
				element->SetDoor(glm::vec4(0, 1.0f, 0, 0));
			}
			if (walls.w == 0)
			{
				element->SetDoor(glm::vec4(0, 0, 0, 1.0f));
			}
		}

		if (element->Walls.y == 1.0f && element->Walls.w == 1.0f)
		{
			flag_door = true;
			if (walls.x == 0)
			{
				element->SetDoor(glm::vec4(1.0f, 0, 0, 0));
			}
			if (walls.z == 0)
			{
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
	SceneNode *mapRoot = new SceneNode();
	for (int i = 0; i < maps.size(); i++)
	{
		mapRoot->AddChild(maps[i]->GenerateNode(nodes, mapRoot, floor, wall, door));
	}
	nodes.push_back(mapRoot);
}

glm::vec2 MapGenerator::GetVector2(int step)
{
	glm::vec2 pos_add;
	while (!CheckIfAvailiable(positions[step] + pos_add))
	{
		int move = GetDirection();
		switch (move)
		{
			default:
			{
				step = maps[step]->ParentElement;
				break;
			}
			case 1: case 10:
			{
				pos_add = glm::vec2(0, 1.0f);
				break;
			}
			case 2: case 11:
			{
				pos_add = glm::vec2(1.0f, 0);
				break;
			}
			case 3: case 12:
			{
				pos_add = glm::vec2(0, -1.0f);
				break;
			}
			case 4: case 13:
			{
				pos_add = glm::vec2(-1.0f, 0);
				break;
			}
		}
	}
	return pos_add + this->positions[step];
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
		return glm::vec4(0, 1.0f, 0, 0);
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

MapElement *MapGenerator::GetElement(glm::vec2 pos)
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
	for (std::vector<MapElement *>::iterator door = doors.begin(); door != doors.end(); door++)
	{
		MapElement *element = *door;
		if (element->DoesHaveADoor())
		{
			std::vector<glm::vec2> neighbours = element->GetNeighbours();
			for (int j = 0; j < neighbours.size(); j++)
			{
				for (std::vector<MapElement *>::iterator it = doors.begin(); it != doors.end(); it++)
				{
					MapElement *neighbour = *it;
					if (neighbour->Position == neighbours[j] && neighbour->DoesHaveADoor())
						neighbour->RemoveDoor(element->Doors);
				}
			}
		}
	}
}

void MapGenerator::ClearDoors()
{
	std::vector<MapElement *>::iterator it = doors.begin();
	do
	{
		MapElement *temp = *it;
		if (!temp->DoesHaveADoor())
			it = doors.erase(it);
		else
			it++;
	} while (it != doors.end());
}

int MapGenerator::CountDoors()
{
	int count = 0;
	for (std::vector<MapElement *>::iterator door = doors.begin(); door != doors.end(); door++)
	{
		MapElement *el = *door;
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
		if (doorsCount - count >= Doors)
		{
			doors[index]->CleanDoors();
			doorsCount -= count;
		}
		else
		{
			doors[index]->RemoveDoor(GetRandomIndex(1));
			doorsCount--;
		}
	} while (Doors != doorsCount);
}

int MapGenerator::GetDirection()
{
	return (rand() % 10);
}

int MapGenerator::GetRandomIndex(int max)
{
	return (rand() % max);
}

void MapGenerator::FillWithNull(int max)
{
	for (int i = 0; i < max; i++)
	{
		for (int j = 0; j < max; j++)
		{
			if (CheckIfNull(i, j))
			{
				mapped.push_back(new MapKey(i, j, MapType::Null));
			}
		}
	}
}

void MapGenerator::TransformToPositive()
{

	int minX = 0;
	int minY = 0;

	for (auto map : maps)
	{
		if (map->Position.x < minX)
			minX = map->Position.x;
		if (map->Position.y < minY)
			minY = map->Position.y;
	}

	if (minX <= 0 && minY <= 0)
	{
		for (auto &pos : maps)
		{
			pos->Position.x += abs(minX)+1; //+1 zeby zostało miejsce na sciane
			pos->Position.y += abs(minY)+1; //tak samo
		}
	}
}

bool MapGenerator::CheckIfNull(int x, int y)
{
	for (int i = 0; i < mapped.size(); i++)
	{
		if (mapped[i]->x == x && mapped[i]->y == y)
			return false;
	}
	return true;
}