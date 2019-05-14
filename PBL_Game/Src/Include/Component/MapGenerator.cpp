#include "Component/MapGenerator.hpp"

MapGenerator::MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram)
{
	this->nodes = nodes;
	this->shader = shaderProgram;
	srand(time(NULL));
	GenerateMap(4);
	CheckForWallsNDoors();
	FinishGeneration();
}

MapGenerator::MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram, int squares, int doors, bool glass_doors)
{
	this->nodes = nodes;
	this->shader = shaderProgram;
	srand(time(NULL));
	GenerateMap(squares);
	CheckForWallsNDoors();
	FinishGeneration();
}

void MapGenerator::GenerateMap(int n)
{
	MapElement element(glm::vec2(0, 0), shader);
	map.insert(std::pair<glm::vec2, MapElement>(element.Position, element));
	for (int i = 0; i < n-1; i++)
	{
		glm::vec2 pos = map[map.rbegin()->first].Position;
		MapElement element(pos + GetVector2(pos), shader);
		map.insert(std::pair<glm::vec2, MapElement>(element.Position, element));
	}
}

int MapGenerator::GetDirection()
{
	return (rand() % 4 + 1);
}

void MapGenerator::CheckForWallsNDoors()
{
	for (std::map<glm::vec2, MapElement>::iterator it = map.begin(); it != map.end(); ++it)
	{
		std::vector<glm::vec2> neighbours = it->second.GetNeighbours();
		for (std::vector<glm::vec2>::iterator neighbour = neighbours.begin(); neighbour != neighbours.end(); neighbour++)
		{
			if (map.find(*neighbour) == map.end())
				it->second.SetWall(GetVector4(it->second.Position - *neighbour));
			else
				it->second.SetDoor(GetVector4(it->second.Position - *neighbour));
		}
	}
}

void MapGenerator::FinishGeneration()
{
	SceneNode mapRoot;
	for (std::map<glm::vec2, MapElement>::iterator element = map.begin(); element != map.end(); ++element)
	{
		element->second.GenerateNode(nodes, &mapRoot);
	}
}

glm::vec2 MapGenerator::GetVector2(glm::vec2 pos)
{
	glm::vec2 pos_add;
	while (!CheckIfAvailiable(pos + pos_add)) {
		int move = GetDirection();
		switch (move)
		{
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
	return pos_add;
}

glm::vec4 MapGenerator::GetVector4(glm::vec2 direction)
{
	switch ((int)direction.x)
	{
		case 1:
		{
			return glm::vec4(0, 1.0f, 0, 0);
		}
		case -1:
		{
			return glm::vec4(0, 0, 0, 1.0f);
		}
	}

	switch ((int)direction.y)
	{
		case 1:
		{
			return glm::vec4(1.0f, 0, 0, 0);
		}
		case -1:
		{
			return glm::vec4(0, 0, 1.0f, 0);
		}
	}
}

bool MapGenerator::CheckIfAvailiable(glm::vec2 pos)
{
	return map.find(pos) == map.end();
}