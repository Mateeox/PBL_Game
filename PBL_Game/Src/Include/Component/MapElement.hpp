#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "SceneNode.hpp"

class MapElement {
	private:
	std::vector<SceneNode>* nodes;
	SceneNode* parent;
	const float scale = 1.0f;

	public:
	glm::vec2 Position;
	glm::vec2 Walls;
	glm::vec2 Doors;

	MapElement(glm::vec2 pos);
	void SetWall(glm::vec2 wall);
	void SetDoor(glm::vec2 door);
	std::vector<glm::vec2> GetNeighbours();
	SceneNode* GenerateNode(std::vector<SceneNode>* nodes, SceneNode* parent);
	SceneNode* AddFloor();
	SceneNode* AddWalls(int n);
};