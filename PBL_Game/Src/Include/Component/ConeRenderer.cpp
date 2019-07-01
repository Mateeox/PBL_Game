#include "ConeRenderer.hpp"
const double ConeRenderer::EPSILON = 1e-6;

ConeRenderer::ConeRenderer(Shader &shaderProgram, SceneNode *rootNode) : Drawable(shaderProgram), rootNode(rootNode)
{
	anglePerSegment = angle / segmentsNumber;
	const int pointsInfinalVerticesNum = (segmentsNumber + 2) * 3;
	finalVertices.reserve(pointsInfinalVerticesNum);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind vertex Arrary object
	glBindVertexArray(VAO);
	//bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, pointsInfinalVerticesNum * sizeof(float), finalVertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
}

ComponentSystem::ComponentType ConeRenderer::GetComponentType()
{
	return ComponentSystem::ComponentType::ConeRenderer;
}

void ConeRenderer::Draw(Shader *shader, glm::mat4 &transform)
{
	// if (shader == nullptr)
	// {
	// 	defaultShader.use();
	// }
	// else
	// {
	// 	shader->use();
	// }

	// glm::vec2 startPoint(transform[3][0], transform[3][2]);
	// glm::vec2 endPointLeft = rotatePointAroundPoint(glm::vec2(startPoint.x + radius, startPoint.y), startPoint, directionAngle);
	// glm::vec2 endPointRight = rotatePointAroundPoint(endPointLeft, startPoint, angle);
	// double radiusSquared = radius * radius;

	// objectsInCone.clear();
	// //don't judge me, this workaround is 10x faster than rewriting everything
	// std::vector<SceneNode *> rootVector{rootNode};
	// collectObjectsInCone(startPoint, endPointLeft, endPointRight, &rootVector, Transform::origin(), radiusSquared);
	// glm::vec2 right(endPointLeft);
	// finalVertices.clear();
	// addToFinalVertices(startPoint);

	// auto pointsAfterTransformation = calculatePointsAfterTransformation(objectsInCone);
	// for (int i = 0; i < segmentsNumber; ++i)
	// {
	// 	glm::vec2 left(right);
	// 	right = rotatePointAroundPoint(left, startPoint, anglePerSegment);
	// 	if (!pointsAfterTransformation.empty())
	// 	{
	// 		double currentSmallestDistanceSquared = radiusSquared;

	// 		for (auto points : pointsAfterTransformation)
	// 		{
	// 			for (unsigned int j = 0; j < points.size(); j += 3)
	// 			{
	// 				checkIntersection(currentSmallestDistanceSquared, startPoint, right, points[j], points[j + 1]);
	// 				checkIntersection(currentSmallestDistanceSquared, startPoint, right, points[j + 1], points[j + 2]);
	// 				checkIntersection(currentSmallestDistanceSquared, startPoint, right, points[j + 2], points[j]);
	// 			}
	// 		}

	// 		if (currentSmallestDistanceSquared < radiusSquared)
	// 		{
	// 			auto currentRadius = glm::sqrt(currentSmallestDistanceSquared);
	// 			left = rotatePointAroundPoint(glm::vec2(startPoint.x + currentRadius, startPoint.y), startPoint, directionAngle + anglePerSegment * i);
	// 			if (i == (segmentsNumber - 1))
	// 			{
	// 				right = rotatePointAroundPoint(left, startPoint, anglePerSegment);
	// 			}
	// 		}
	// 	}
	// 	addToFinalVertices(left);
	// }
	// addToFinalVertices(right);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferSubData(GL_ARRAY_BUFFER, 0, finalVertices.size() * sizeof(float), finalVertices.data());

	// glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, finalVertices.size() / 3);
}

void ConeRenderer::rotateRight()
{
	double tmpAngle = directionAngle + rotationSpeed;
	if (tmpAngle > glm::two_pi<double>())
		tmpAngle -= glm::two_pi<double>();
	directionAngle = tmpAngle;
}

void ConeRenderer::rotateLeft()
{
	double tmpAngle = directionAngle - rotationSpeed;
	if (tmpAngle < 0)
		tmpAngle += glm::two_pi<double>();
	directionAngle = tmpAngle;
}

double ConeRenderer::getDirectionAngle()
{
	return directionAngle;
}

double ConeRenderer::getAngle()
{
	return angle;
}

bool ConeRenderer::isOnLeftSide(const glm::vec2 &startPoint, const glm::vec2 &endPoint, const glm::vec2 &pointToCheck)
{
	return ((endPoint.x - startPoint.x) * (pointToCheck.y - startPoint.y) - (endPoint.y - startPoint.y) * (pointToCheck.x - startPoint.x)) < 0;
}
bool ConeRenderer::areLinesIntersecting(const glm::vec2 &aLineStart, const glm::vec2 &aLineEnd, const glm::vec2 &bLineStart, const glm::vec2 &bLineEnd)
{
	float ax = aLineEnd.x - aLineStart.x; // direction of line a
	float ay = aLineEnd.y - aLineStart.y; // ax and ay as above

	float bx = bLineStart.x - bLineEnd.x; // direction of line b, reversed
	float by = bLineStart.y - bLineEnd.y; // really -by and -by as above

	float dx = bLineStart.x - aLineStart.x; // right-hand side
	float dy = bLineStart.y - aLineStart.y;

	float det = ax * by - ay * bx;

	if (abs(det) < ConeRenderer::EPSILON)
		return false;

	float r = (dx * by - dy * bx) / det;
	float s = (ax * dy - ay * dx) / det;

	return !(r < 0 || r > 1 || s < 0 || s > 1);
}
bool ConeRenderer::findLinesIntersectionPoint(glm::vec2 &intersectionPoint, const glm::vec2 &startPointA, const glm::vec2 &endPointA, const glm::vec2 &startPointB, const glm::vec2 &endPointB)
{
	auto r = endPointA - startPointA;
	auto s = endPointB - startPointB;
	auto rxs = glm::determinant(glm::mat2(r, s));
	auto qpxr = glm::determinant(glm::mat2(startPointB - startPointA, r));

	if (abs(rxs) < ConeRenderer::EPSILON)
		return false;

	auto t = glm::determinant(glm::mat2(startPointB - startPointA, s)) / rxs;
	auto u = qpxr / rxs;

	if (abs(rxs) > ConeRenderer::EPSILON && (0 <= t && t <= 1) && (0 <= u && u <= 1))
	{
		intersectionPoint = startPointA + t * r;
		return true;
	}
	return false;
}

void ConeRenderer::collectObjectsInCone(const glm::vec2 &startPoint, const glm::vec2 &endPointLeft, const glm::vec2 &endPointRight, std::vector<SceneNode *> *nodes, Transform transform, double radiusSquared)
{
	if (radiusSquared < 0)
	{
		//if the radius is negative it means it was not given, so we have to calculate it from points
		radiusSquared = glm::distance2(endPointLeft, startPoint);
	}
	for (auto node : *nodes)
	{
		Transform combinedTransform;
		bool isTransformsCombined = false;
		if (node->gameObject != nullptr && node->gameObject->getTag() != "player" && node->gameObject->getTag() != "floor")
		{
			ConeRenderable *shapeRenderer = (ConeRenderable *)node->gameObject->GetComponent(ComponentSystem::ComponentType::ShapeRenderer3D);
			if (shapeRenderer != nullptr)
			{
				combinedTransform = node->gameObject->transform.combine(transform);
				collectObjectsInConeInternal(shapeRenderer, startPoint, endPointLeft, endPointRight, node, combinedTransform, isTransformsCombined, radiusSquared);
			}
			else
			{
				Model *model = (Model *)node->gameObject->GetComponent(ComponentSystem::ComponentType::Model);
				if (model != nullptr)
				{
					combinedTransform = node->gameObject->transform.combine(transform);
					for (auto &mesh : *model->getMeshes())
					{
						collectObjectsInConeInternal(&mesh, startPoint, endPointLeft, endPointRight, node, combinedTransform, isTransformsCombined, radiusSquared);
					}
				}
			}
		}
		collectObjectsInCone(startPoint, endPointLeft, endPointRight, &node->children, isTransformsCombined ? combinedTransform : transform, radiusSquared);
	}
}

bool ConeRenderer::collectObjectsInConeInternal(ConeRenderable *shapeRenderer, const glm::vec2 &startPoint, const glm::vec2 &endPointLeft, const glm::vec2 &endPointRight, SceneNode *node, Transform &combinedTransform, bool &isTransformsCombined, double radiusSquared)
{
	auto points = shapeRenderer->getExtrema();
	isTransformsCombined = true;
	std::vector<glm::vec2> pointsAfterTransfomration;
	pointsAfterTransfomration.reserve(points.size());
	for (auto point : points)
	{
		auto transformedPoint = combinedTransform.GetTransform() * point;
		if (transformedPoint.y < 0)
		{
			//lets omit objects that are below the surface
			return false;
		}
		auto pointCastedToXZPlane = glm::vec2(transformedPoint.x, transformedPoint.z);
		pointsAfterTransfomration.push_back(pointCastedToXZPlane);

		if (glm::distance2(pointCastedToXZPlane, startPoint) < radiusSquared)
		{
			//vertex of object is within cone radius, now lets check if it is between borders
			if (!isOnLeftSide(startPoint, endPointLeft, pointCastedToXZPlane) && isOnLeftSide(startPoint, endPointRight, pointCastedToXZPlane))
			{
				objectsInCone.push_back(std::make_pair(shapeRenderer, combinedTransform.GetTransform()));
				return true;
			}
		}
	}
	//it is still possible that an object intersects the cone, but none of its vertices are inside
	for (int i = 0; i < pointsAfterTransfomration.size() - 1; ++i)
	{
		int nextPointIndex = (i + 1) % pointsAfterTransfomration.size();
		//check left border
		if (areLinesIntersecting(startPoint, endPointLeft, pointsAfterTransfomration[i], pointsAfterTransfomration[nextPointIndex]))
		{
			objectsInCone.push_back(std::make_pair(shapeRenderer, combinedTransform.GetTransform()));
			return true;
		}
		//check right border
		if (areLinesIntersecting(startPoint, endPointRight, pointsAfterTransfomration[i], pointsAfterTransfomration[nextPointIndex]))
		{
			objectsInCone.push_back(std::make_pair(shapeRenderer, combinedTransform.GetTransform()));
			return true;
		}
		//check middle line (for an object intersecting with the curved area of the cone)
		if (areLinesIntersecting(startPoint, rotatePointAroundPoint(endPointLeft, startPoint, angle / 2.0), pointsAfterTransfomration[i], pointsAfterTransfomration[nextPointIndex]))
		{
			objectsInCone.push_back(std::make_pair(shapeRenderer, combinedTransform.GetTransform()));
			return true;
		}
	}
	return false;
}

std::vector<std::vector<glm::vec2>> ConeRenderer::calculatePointsAfterTransformation(const std::vector<std::pair<ConeRenderable *, glm::mat4>> &objects)
{
	std::vector<std::vector<glm::vec2>> results;
	for (auto obj : objects)
	{
		std::vector<glm::vec2> points;
		float *vertexData = obj.first->getVertexData();
		int verticesPositionStride = obj.first->getVerticesPositionStride();
		int indicesSize = obj.first->getIndicesSize();
		if (indicesSize != 0)
		{
			unsigned int *indices = obj.first->getIndices();
			for (unsigned int i = 0; i < indicesSize; ++i)
			{
				unsigned int index = indices[i] * verticesPositionStride;
				glm::vec4 transformedPoint(vertexData[index], vertexData[index + 1], vertexData[index + 2], 1);
				transformedPoint = obj.second * transformedPoint;
				points.push_back(glm::vec2(transformedPoint.x, transformedPoint.z));
			}
		}
		else
		{
			int vertexDataSize = obj.first->getVertexDataSize();
			for (int i = 0; i <= vertexDataSize; i += verticesPositionStride)
			{
				glm::vec4 transformedPoint(vertexData[i], vertexData[i + 1], vertexData[i + 2], 1.0);
				transformedPoint = obj.second * transformedPoint;
				points.push_back(glm::vec2(transformedPoint.x, transformedPoint.z));
			}
		}
		results.push_back(points);
	}
	return results;
}

void ConeRenderer::addToFinalVertices(glm::vec2 point)
{
	finalVertices.push_back(point.x);
	finalVertices.push_back(coneHeight);
	finalVertices.push_back(point.y);
}

void ConeRenderer::checkIntersection(double &distance, const glm::vec2 &startPointA, const glm::vec2 &endPointA, const glm::vec2 &startPointB, const glm::vec2 &endPointB)
{
	glm::vec2 intersectionPoint;
	if (findLinesIntersectionPoint(intersectionPoint, startPointA, endPointA, startPointB, endPointB))
	{
		double tmpDistSq = glm::distance2(intersectionPoint, startPointA);
		if (tmpDistSq < distance)
		{
			distance = tmpDistSq;
		}
	}
}

glm::vec2 ConeRenderer::rotatePointAroundPoint(glm::vec2 point, glm::vec2 center, double angle)
{
	auto dx = point.x - center.x;
	auto dy = point.y - center.y;
	return glm::vec2(
		cos(angle) * (dx)-sin(angle) * (dy) + center.x,
		sin(angle) * (dx) + cos(angle) * (dy) + center.y);
}
