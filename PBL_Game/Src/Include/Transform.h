#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;
class Transform {

bool enable;
vec3 position;
vec3 rotation;
vec3 scale;
mat4 transfrom;
quat quaterion;
mat4 rotationMatrix;


public:
	Transform();
	//mat4 Transform();
	Transform Combine(Transform& other);
	vec3 Rotation();
	vec3 Scale();
	quat Quaterion();
	mat4 RotationMatrix();
	void SetScale(vec3 scale);
	void SetRotate(float value, vec3 axis);
	void SetPosition(vec3 position);
	void Rotate();
	void Translate();
};