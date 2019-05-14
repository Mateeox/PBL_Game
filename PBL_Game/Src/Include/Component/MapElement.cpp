#include "MapElement.hpp"

MapElement::MapElement(glm::vec2 pos, Shader* shaderProgram)
{
	this.Position = pos;
	this->shader = shaderProgram;
}

void MapElement::SetWall(glm::vec4 wall)
{
	Walls += wall;
}

void MapElement::SetDoor(glm::vec4 door)
{
	Doors += door;
}

std::vector<glm::vec2> MapElement::GetNeighbours()
{
	std::vector<glm::vec2> neighbours;
	neighbours.push_back(this.Position + glm::vec2(1.0f, 0));
	neighbours.push_back(this.Position + glm::vec2(-1.0f, 0));
	neighbours.push_back(this.Position + glm::vec2(0, 1.0f));
	neighbours.push_back(this.Position + glm::vec2(0, -1.0f));
	return neighbours;
}

SceneNode* MapElement::GenerateNode(std::vector<SceneNode>* nodes, SceneNode* parent)
{
	SceneNode element;
	
}

SceneNode* MapElement::AddFloor()
{
	return nullptr;
}

SceneNode* MapElement::CreateWall(SceneNode* parent, float direction_x, float direction_y)
{
	return nullptr;
}

SceneNode* MapElement::CreateDoor(SceneNode* parent, float direction_x, float direction_y)
{
	return nullptr;
}

void MapElement::AddWalls(SceneNode* node)
{
}

void MapElement::AddDoors(SceneNode* node)
{
}
