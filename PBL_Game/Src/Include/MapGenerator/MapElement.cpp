#include "MapGenerator/MapElement.hpp"

MapElement::MapElement(std::vector<SceneNode*>&aNodes):nodes(aNodes) {
	this->Position = glm::vec2();
	this->Doors = glm::vec4();
}

MapElement::MapElement(glm::vec2 pos, std::vector<SceneNode*>& aNodes, int ParentElement) : nodes(aNodes)
{
	this->Position = glm::vec2(pos.x, pos.y);
	this->ParentElement = ParentElement;
	this->Doors = glm::vec4();
	this->mirror = mirror;
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

SceneNode* MapElement::GenerateNode(std::vector<SceneNode*>& aNodes, SceneNode* parent, Model* floorMod, Model* wallMod, Model* doorMod, Model* keyMod, int& door_index, bool mirror)
{
	SceneNode* element = new SceneNode();
	element->AddParent(parent);
	SceneNode* floor = AddFloor(floorMod);
	floor->AddParent(element);
	element->AddChild(floor);
	std::vector<SceneNode*> walls = AddWalls(floor, wallMod);
	for (int i = 0; i < walls.size(); i++)
		element->AddChild(walls[i]);
	walls.clear();
	std::vector<SceneNode*> doors = AddDoors(floor, doorMod, keyMod, door_index);
	for (int i = 0; i < doors.size(); i++)
		element->AddChild(doors[i]);
	nodes.push_back(element);
	return element;
}

SceneNode* MapElement::AddFloor(Model* model)
{
	SceneNode* floor = new SceneNode();
	GameObject* oFloor = new GameObject(floor->local);
	oFloor->AddComponent(model);
	floor->AddGameObject(oFloor);
	floor->Translate(Position.x, 0, Position.y);
	//floor->Rotate(-90.0f, glm::vec3(1, 0, 0));
	floor->Scale(0.0254f, 0.0254f, 0.0254f);
	nodes.push_back(floor);
	return floor;
}

SceneNode* MapElement::CreateWall(SceneNode* parent, Model* model, float direction_x, float direction_y)
{
	SceneNode* wall = new SceneNode();
	GameObject* wallObj = new GameObject(wall->local);
	wallObj->AddComponent(model);
	wall->AddGameObject(wallObj);
	wall->Translate(Position.x + direction_x * wall_offset, 0, Position.y + direction_y * wall_offset);
	wall->Rotate(direction_y == 0 ? 90.0f : 0, glm::vec3(0, 1, 0));
	wall->Scale(0.0254f, 0.0254f, 0.01f);
	
	wall->AddParent(parent);
	return wall;
}

SceneNode* MapElement::CreateDoor(SceneNode* parent, Model* model, Model* key, int& door_index, float direction_x, float direction_y)
{
	if (!mirror) {
		keydoor = KeyDoorFactory::Create(door_index, model, key);
		SceneNode* door = keydoor.second;
		door->Translate(Position.x + direction_x * wall_offset, 0, Position.y + direction_y * wall_offset);
		door->Rotate(direction_y == 0 ? -90.0f : 0, glm::vec3(0, 1, 0));
		door->Scale(0.0254f, 0.0254f, 0.01f);
		door->AddParent(parent);
		door_index++;
		return door;
	}
	else
	{
		SceneNode* key = keydoor.first;
		key->Translate(Position.x, 0, Position.y);
		key->Scale(0.0254f, 0.0254f, 0.01f);
		key->AddParent(parent);
		return key;
	}
}

std::vector<SceneNode*> MapElement::AddWalls(SceneNode* node, Model* model)
{
	std::vector<SceneNode*> temp;
	if (Walls.x > 0)
		temp.push_back(CreateWall(node, model, 0, 1.0f));
	if (Walls.y > 0)
		temp.push_back(CreateWall(node, model, 1.0f, 0));
	if (Walls.z > 0)
		temp.push_back(CreateWall(node, model, 0, -1.0f));
	if (Walls.w > 0)
		temp.push_back(CreateWall(node, model, -1.0f, 0));
	return temp;
}

std::vector<SceneNode*> MapElement::AddDoors(SceneNode* node, Model* model, Model* key, int& index_door)
{
	std::vector<SceneNode*> temp;
	if (Doors.x > 0)
		temp.push_back(CreateDoor(node, model, key, index_door, 0, 1.0f));
	if (Doors.y > 0)
		temp.push_back(CreateDoor(node, model, key, index_door, 1.0f, 0));
	if (Doors.z > 0)
		temp.push_back(CreateDoor(node, model, key, index_door, 0, -1.0f));
	if (Doors.w > 0)
		temp.push_back(CreateDoor(node, model, key, index_door, -1.0f, 0));
	return temp;
}

bool MapElement::DoesHaveADoor()
{
	if (Doors.x == 1.0f || Doors.y == 1.0f || Doors.z == 1.0f || Doors.w == 1.0f)
		return true;
	else
		return false;
}

int MapElement::CountDoors()
{
	int count = 0;
	if (Doors.x == 1.0f)
		count++;
	if (Doors.y == 1.0f)
		count++;
	if (Doors.z == 1.0f)
		count++;
	if (Doors.w == 1.0f)
		count++;
	return count;
}

void MapElement::RemoveDoor(glm::vec4 doors)
{
	Doors -= doors;
	if (Doors.x == -1.0f)
		Doors.x = 0;
	if (Doors.y == -1.0f)
		Doors.y = 0;
	if (Doors.z == -1.0f)
		Doors.z = 0;
	if (Doors.w == -1.0f)
		Doors.w = 0;
}

void MapElement::RemoveDoor(int order)
{
	if (Doors.x == 1.0f && order == 0)
		Doors.x = 0;
	if (Doors.y == 1.0f && order == 0)
		Doors.y = 0;
	if (Doors.z == 1.0f && order == 1)
		Doors.z = 0;
	if (Doors.w == 1.0f && order == 1)
		Doors.w = 0;
}

void MapElement::CleanDoors()
{
	Doors = glm::vec4();
}