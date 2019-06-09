#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../SceneNode.hpp"
#include "Shapes.hpp"
#include "Component/Model.hpp"
#include "KeyDoorFactory.hpp"
class MapElement {
	private:
	std::vector<SceneNode*>& nodes;
	SceneNode* parent;
	float scale = 1.0f;
	float floor_offset = 1.0f;
	float wall_offset = 0.5f;
	bool mirror = false;
	std::pair<SceneNode*, SceneNode*> keydoor;
	std::vector<SceneNode*>* sNodes;

	public:
	glm::vec2 Position;
	glm::vec4 Walls;
	glm::vec4 Doors;
	bool Chest = false;
	int ParentElement;
	MapElement(std::vector<SceneNode*>& aNodes);
	MapElement(glm::vec2 pos,std::vector<SceneNode*>& aNodes, int ParentElement = 0);
	void SetWall(glm::vec4 wall);
	void SetDoor(glm::vec4 door);
	bool DoesHaveADoor();
	int CountDoors();
	void CleanDoors();
	void RemoveDoor(glm::vec4 doors);
	void RemoveDoor(int order);
	std::vector<glm::vec2> GetNeighbours();
	SceneNode* GenerateNode(std::vector<SceneNode*>& nodes, SceneNode* parent, Model* floor, Model* wall, Model* door, Model* key, Model* chest, int& door_index, std::vector<SceneNode*>* sNodes, bool mirror = false);
	SceneNode* AddFloor(Model* model);
	std::vector<SceneNode*> AddWalls(SceneNode* node, Model* model);
	std::vector<SceneNode*> AddDoors(SceneNode* node, Model* model, Model* key, int& door_index);
	SceneNode* CreateWall(SceneNode* parent, Model* model, float direction_x, float direction_y);
	SceneNode* CreateDoor(SceneNode* parent, Model* model, Model* key, int& door_index, float direction_x, float direction_y);
	SceneNode* CreateChest(SceneNode* parent, Model* model);
};