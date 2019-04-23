#include "MapElement.hpp"

MapElement::MapElement(glm::vec2 pos)
{
	Position = pos;
}

void MapElement::SetWall(glm::vec4 wall)
{
	Walls += wall;
}

void MapElement::SetDoor(glm::vec4 door)
{
	Doors += door;
}