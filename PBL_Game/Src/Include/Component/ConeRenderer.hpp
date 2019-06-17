#pragma once
#include "ShapeRenderer3D.hpp"
#include "Model.hpp"
#include "SceneNode.hpp"
#include <vector>
#include <array>
#include <utility>

class ConeRenderer : public Drawable
{
	SceneNode* rootNode;
	double anglePerSegment;
	double radius = 5.0;
	double directionAngle = 3;
	const int segmentsNumber = 10;
	const double angle = 1.2;
	const double coneHeight = 0.2;
	const double rotationSpeed = 0.1;
	std::vector<float> finalVertices;
	std::vector<std::pair<ConeRenderable*, glm::mat4 >> objectsInCone;

	bool isOnLeftSide(const glm::vec2 &startPoint, const glm::vec2 &endPoint, const glm::vec2 &pointToCheck);
	bool areLinesIntersecting(const glm::vec2& aLineStart, const glm::vec2& aLineEnd, const glm::vec2& bLineStart, const glm::vec2& bLineEnd);
	bool findLinesIntersectionPoint(glm::vec2 &intersectionPoint, const glm::vec2 &startPointA, const glm::vec2 &endPointA, const glm::vec2 &startPointB, const glm::vec2 &endPointB);
	void collectObjectsInCone(const glm::vec2 &startPoint, const glm::vec2 &endPointLeft, const glm::vec2 &endPointRight, std::vector<SceneNode*> *nodes, Transform transform = Transform::origin(), double radiusSquared = -1.0);
	std::vector<std::vector<glm::vec2>> calculatePointsAfterTransformation(const std::vector<std::pair<ConeRenderable*, glm::mat4 >> &objects);
	void addToFinalVertices(glm::vec2 point);
	void checkIntersection(double &distance, const glm::vec2 &startPointA, const glm::vec2 &endPointA, const glm::vec2 &startPointB, const glm::vec2 &endPointB);
	glm::vec2 rotatePointAroundPoint(glm::vec2 point, glm::vec2 center, double angle);
	bool collectObjectsInConeInternal(ConeRenderable* shapeRenderer, const glm::vec2 &startPoint, const glm::vec2 &endPointLeft, const glm::vec2 &endPointRight, SceneNode* node, Transform& combinedTransform, bool& isTransformsCombined, double radiusSquared);

public:
	ConeRenderer(Shader &shaderProgram, SceneNode* rootNode);
	ComponentSystem::ComponentType GetComponentType() override;
	void Draw(glm::mat4 &transform) override;
	void rotateRight();
	void rotateLeft();
	double getDirectionAngle();
	double getAngle();

	static const double EPSILON;
};