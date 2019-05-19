#include "MapElement.hpp"

MapElement::MapElement() {
	this->Position = glm::vec2();
}

MapElement::MapElement(glm::vec2 pos, Shader* shaderProgram)
{
	this->Position = glm::vec2(pos.x, pos.y);
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
	neighbours.push_back(this->Position + glm::vec2(1.0f, 0));
	neighbours.push_back(this->Position + glm::vec2(-1.0f, 0));
	neighbours.push_back(this->Position + glm::vec2(0, 1.0f));
	neighbours.push_back(this->Position + glm::vec2(0, -1.0f));
	return neighbours;
}

SceneNode* MapElement::GenerateNode(std::vector<SceneNode*>* nodes, SceneNode* parent)
{
	this->nodes = nodes;
	SceneNode* element = new SceneNode();
	element->AddParent(parent);
	SceneNode* floor = AddFloor();
	floor->AddParent(element);
	element->AddChild(floor);
	nodes->push_back(element);
	return element;
}

SceneNode* MapElement::AddFloor()
{
	SceneNode* floor = new SceneNode();
	GameObject* oFloor = new GameObject(floor->local);
	Texture* xD = new Texture("Textures/red.png", GL_LINEAR);
	xD->Load();
	ShapeRenderer3D* Floor = new ShapeRenderer3D(Shapes::RainBow_Square,
		Shapes::RB_Square_indices,
		sizeof(Shapes::RainBow_Square),
		sizeof(Shapes::RB_Square_indices),
		*shader,
		xD);
	oFloor->AddComponent(Floor);
	floor->AddGameObject(oFloor);
	floor->AddChild(floor);
	floor->Translate(Position.x, Position.y, 0);
	nodes->push_back(floor);
	return floor;
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
