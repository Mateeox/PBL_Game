#pragma once
#include <glm/glm.hpp>

class MapElement {
	public:
	glm::vec2 Position;
	glm::vec4 Walls;
	glm::vec4 Doors;

	MapElement(glm::vec2 pos);
	void SetWall(glm::vec4 wall);
	void SetDoor(glm::vec4 door);
};