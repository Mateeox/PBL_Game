#pragma once
#include "MapElement.hpp"
#include <map>
#include <time.h>

class MapGenerator //: public Script
{
	private:
	std::map <glm::vec2, MapElement> map;
	std::vector<SceneNode*>* nodes;
	Shader* shader;

	int GetDirection();
	void CheckForWallsNDoors();
	void GenerateMap(int n);
	void FinishGeneration();
	bool CheckIfAvailiable(glm::vec2 pos);
	glm::vec2 GetVector2(glm::vec2 pos);
	glm::vec4 GetVector4(glm::vec2 direction);

	public:
	MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram);
	MapGenerator(std::vector<SceneNode*>* nodes, Shader* shaderProgram, int squares, int doors, bool glass_door);
};