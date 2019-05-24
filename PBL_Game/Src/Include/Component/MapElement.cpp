#include "MapElement.hpp"

MapElement::MapElement() {
	this->Position = glm::vec2();
}

MapElement::MapElement(glm::vec2 pos, Shader* shaderProgram, int ParentElement)
{
	this->Position = glm::vec2(pos.x, pos.y);
	this->shader = shaderProgram;
	this->ParentElement = ParentElement;
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
	neighbours.push_back(this->Position + glm::vec2(0, 1.0f));
	neighbours.push_back(this->Position + glm::vec2(1.0f, 0));
	neighbours.push_back(this->Position + glm::vec2(0, -1.0f));
	neighbours.push_back(this->Position + glm::vec2(-1.0f, 0));
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
	std::vector<SceneNode*> walls = AddWalls(floor);
	int size = walls.size();
	for (int i = 0; i < walls.size(); i++)
		element->AddChild(walls[i]);
	walls.clear();
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
	floor->Translate(Position.x, 0, Position.y);
	floor->Rotate(-90.0f, glm::vec3(1, 0, 0));
	nodes->push_back(floor);
	return floor;
}

SceneNode* MapElement::CreateWall(SceneNode* parent, float direction_x, float direction_y)
{
	SceneNode* wall = new SceneNode();
	GameObject* wallObj = new GameObject(wall->local);
	Texture* xD = new Texture("Textures/red.png", GL_LINEAR);
	xD->Load();
	ShapeRenderer3D *szescian = new ShapeRenderer3D(Shapes::RainBow_Cube,
		Shapes::RB_Cube_indices,
		sizeof(Shapes::RainBow_Cube),
		sizeof(Shapes::RB_Cube_indices),
		*shader,
		xD);
	wallObj->AddComponent(szescian);
	wall->AddGameObject(wallObj);
	wall->Translate(Position.x + direction_x * wall_offset, 0, Position.y + direction_y * wall_offset);
	wall->Rotate(direction_y != 0 ? -90.0f : 0, glm::vec3(0, 1, 0));
	wall->Scale(0.1f, 0.2f, 0.5f);
	wall->AddParent(parent);
	return wall;
}

SceneNode* MapElement::CreateDoor(SceneNode* parent, float direction_x, float direction_y)
{
	return nullptr;
}

std::vector<SceneNode*> MapElement::AddWalls(SceneNode* node)
{
	std::vector<SceneNode*> temp;
	if (Walls.x > 0)
		temp.push_back(this->CreateWall(node, 0, 1.0f));
	if (Walls.y > 0)
		temp.push_back(this->CreateWall(node, 1.0f, 0));
	if (Walls.z > 0)
		temp.push_back(this->CreateWall(node, 0, -1.0f));
	if (Walls.w > 0)
		temp.push_back(this->CreateWall(node, -1.0f, 0));
	return temp;
}

void MapElement::AddDoors(SceneNode* node)
{
	if (Doors.x > 0)
		this->CreateDoor(node, 0, 1.0f);
	if (Doors.y > 0)
		this->CreateDoor(node, 1.0f, 0);
	if (Doors.z > 0)
		this->CreateDoor(node, 0, -1.0f);
	if (Doors.w > 0)
		this->CreateDoor(node, -1.0f, 0);
}
