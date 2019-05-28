#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../SceneNode.hpp"
#include "Shapes.hpp"
#include "Component/Model.hpp"
class MapElement {
	private:
	std::vector<SceneNode*>* nodes;
	SceneNode* parent;
	float scale = 1.0f;
	float floor_offset = 1.0f;
	float wall_offset = 0.5f;

	public:
	glm::vec2 Position;
	glm::vec4 Walls;
	glm::vec4 Doors;
	int ParentElement;
	MapElement();
	MapElement(glm::vec2 pos, int ParentElement = 0);
	void SetWall(glm::vec4 wall);
	void SetDoor(glm::vec4 door);
	bool DoesHaveADoor();
	int CountDoors();
	void CleanDoors();
	void RemoveDoor(glm::vec4 doors);
	void RemoveDoor(int order);
	std::vector<glm::vec2> GetNeighbours();
	SceneNode* GenerateNode(std::vector<SceneNode*>* nodes, SceneNode* parent, Model* floor, Model* wall, Model* door);
	SceneNode* AddFloor(Model* model);
	SceneNode* CreateWall(SceneNode* parent, Model* model, float direction_x, float direction_y);
	SceneNode* CreateDoor(SceneNode* parent, Model* model, float direction_x, float direction_y);
	std::vector<SceneNode*> AddWalls(SceneNode* node, Model* model);
	std::vector<SceneNode*> MapElement::AddDoors(SceneNode* node, Model* model);
};