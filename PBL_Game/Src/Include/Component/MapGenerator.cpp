#include "MapGenerator.hpp"

MapGenerator::MapGenerator()
{
	srand(time(NULL));
	GenerateMap(4);
}

MapGenerator::MapGenerator(int squares, int doors, bool glass_doors)
{
	srand(time(NULL));
	GenerateMap(squares);
}

void MapGenerator::GenerateMap(int n)
{
	MapElement element(glm::vec2(0, 0));
	map.insert(std::pair<glm::vec2, MapElement>(element.Position, element));
	for (int i = 0; i < n-1; i++)
	{
		glm::vec2 pos = map[map.rbegin()->first].Position;
		MapElement element(pos + glm::vec2(GetDirection(), GetDirection()));
		map.insert(std::pair<glm::vec2, MapElement>(element.Position, element));
	}
}

float MapGenerator::GetDirection()
{
	return (float)(rand() % 2 + (-1));
}