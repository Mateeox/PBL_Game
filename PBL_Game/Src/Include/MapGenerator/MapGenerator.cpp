#include "MapGenerator.hpp"

MapGenerator::MapGenerator(Shader *shaderProgram)
{
	this->shader = shaderProgram;
	DefineModels();
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

MapGenerator::MapGenerator(Shader *shaderProgram, int squares, int doors, int chests, bool glass_doors, std::vector<SceneNode*>* sNodes, Player* player)
{
	this->Squares = squares;
	this->Doors = doors;
	this->Chests = chests;
	this->GlassDoor = glass_doors;
	this->shader = shaderProgram;
	this->sNodes = sNodes;
	this->player = player;
	DefineModels();
	srand(time(NULL));
	GenerateMap(squares);
	CheckForWalls();
	CheckForDoors();
	GenerateChests(chests);
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
	MapElement *element = new MapElement(glm::vec2(0, 0), leftnodes);
	maps.push_back(element);
	positions.push_back(element->Position);
	int elementIteration = 0;
	const int boundry = sqrt(n) * 3 / 4;
	for (int i = 0; i < n - 1; i++)
	{
		int parent = 0;
		MapElement *temp = new MapElement(GetVector2(elementIteration, boundry, parent), leftnodes, parent);
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
	int door_index = 0;
	for (int i = 0; i < maps.size(); i++)
	{
		mapRoot->AddChild(maps[i]->GenerateNode(leftnodes, mapRoot, floor, wall, door, key, chest, door_index, sNodes, player));
	}
	leftnodes.push_back(mapRoot);

	door_index = 0;

	SceneNode *mapRoot2 = new SceneNode();
	for (int i = 0; i < maps.size(); i++)
	{
		mapRoot2->AddChild(maps[i]->GenerateNode(rightnodes, mapRoot, floor, wall, door, key, chest, door_index, sNodes, player, true));
	}
	rightnodes.push_back(mapRoot2);
}

glm::vec2 MapGenerator::GetVector2(int step, int boundry, int &parent)
{
	glm::vec2 pos_add;
	int iterator = 0;
	bool stuck = false;
	while (!CheckIfAvailiable(positions[step] + pos_add) || !(abs(positions[step].x + pos_add.x) <= boundry && abs(positions[step].y + pos_add.y) <= boundry))
	{
		int move = GetDirection();
		if (iterator > 10)
		{
			stuck = true;
			step = FindAnyNeighbour(boundry, move);
		}
		
		switch (move)
		{
			/*default:
			{
				if (!stuck) {
					step = maps[step]->ParentElement;
				}
				break;
			}*/
			case 1:
			{
				pos_add = glm::vec2(0, 1.0f);
				break;
			}
			case 2:
			{
				pos_add = glm::vec2(1.0f, 0);
				break;
			}
			case 3:
			{
				pos_add = glm::vec2(0, -1.0f);
				break;
			}
			case 4:
			{
				pos_add = glm::vec2(-1.0f, 0);
				break;
			}
		}
		iterator++;
	}
	parent = step;
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

void MapGenerator::SingleDoor()
{
	for (MapElement* node : doors)
	{
		if (node->CountDoors() > 1)
		{
			node->RemoveDoor(GetRandomIndex(1));
		}
	}
}

void MapGenerator::PickDoors()
{
	SingleDoor();
	int doorsCount = CountDoors();
	if (Doors > doorsCount) {
		Doors = doorsCount;
		return;
	}
	else if (doorsCount < 0 || doorsCount == NULL) {
		Doors = 0;
		return;
	}
	do
	{
		int index = GetRandomIndex(doors.size());
		int count = doors[index]->CountDoors();
		if (doorsCount - count >= Doors)
		{
			doors[index]->CleanDoors();
			doorsCount -= count;
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

int MapGenerator::FindAnyNeighbour(int boundry, int& move)
{
	for (int i = 0; i < maps.size(); i++)
	{
		std::vector<glm::vec2> neighbours = maps[i]->GetNeighbours();
		for (int j = 0; j < neighbours.size(); j++)
		{
			if (CheckIfAvailiable(neighbours[j]) && abs(neighbours[j].x) <= boundry && abs(neighbours[j].y) <= boundry) {
				move = j + 1;
				return i;
			}
		}
	}
	return 0;
}

void MapGenerator::DefineModels()
{
	floor = new Model("Models/House/StaticNormal_Floor.obj", *shader, false);
	wall = new Model("Models/House/StaticSimpleDestroyedWall.obj", *shader, false);
	door = new Model("Models/House/StaticDoor.obj", *shader, false);
	key = new Model("Models/Key/key.obj", *shader, false);
	chest = new Model("Models/Chest/Chest.obj", *shader, false);
}

void MapGenerator::GenerateChests(int amount)
{
	int chest = amount;
	while(chest != 0)
	{
		int index = GetRandomIndex(maps.size());
		if (!maps[index]->Chest) {
			maps[index]->Chest = true;
			chest--;
		}
	}
}