#pragma once
#include "MapElement.hpp"
#include <map>
#include <time.h>

class MapGenerator //: public Script
{
	private:
		struct Vec2Comp {
			bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const
			{
				return lhs.x < rhs.x ||
					lhs.x == rhs.x && (lhs.y < rhs.y || lhs.y == rhs.y);
			}
		};

	std::map <glm::vec2, MapElement, Vec2Comp> map;
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