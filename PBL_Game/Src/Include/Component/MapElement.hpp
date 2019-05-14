#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "SceneNode.hpp"

class MapElement {
	private:
	std::vector<SceneNode>* nodes;
	SceneNode* parent;
	Shader* shader;
	const float scale = 1.0f;
	const float floor_offset = 1.0f;
	const float wall_offset = 1.0f;

	public:
	glm::vec2 Position;
	glm::vec4 Walls;
	glm::vec4 Doors;

	MapElement(glm::vec2 pos, Shader* shaderProgram);
	void SetWall(glm::vec4 wall);
	void SetDoor(glm::vec4 door);
	std::vector<glm::vec2> GetNeighbours();
	SceneNode* GenerateNode(std::vector<SceneNode>* nodes, SceneNode* parent);
	SceneNode* AddFloor();
	SceneNode* CreateWall(SceneNode* parent, float direction_x, float direction_y);
	SceneNode* CreateDoor(SceneNode* parent, float direction_x, float direction_y);
	void AddWalls(SceneNode* node);
	void AddDoors(SceneNode* node);
};