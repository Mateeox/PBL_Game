#include "Transform.hpp"
#include <iostream>

Transform::Transform()
{
  transform = glm::mat4(1.0f);
  Position = glm::vec3(0.0f);
  Scale = glm::vec3(1.0f);
  Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  Quaterion = glm::quat(Rotation);
}

Transform Transform::origin()
{
  return Transform();
}

Transform Transform::combine(Transform &other)
{
  Transform t;
  t.transform = other.transform * transform;
  return t;
}

glm::mat4 & Transform::GetTransform()
{
  return transform;
}

void Transform::ScaleTransform(GLfloat x, GLfloat y, GLfloat z)
{
  Scale *= glm::vec3(x, y, z);
  transform = glm::scale(transform, glm::vec3(x, y, z));
}

void Transform::Translate(glm::vec3 Value)
{

  Position += Value;
  transform = glm::translate(transform, Value);
}

void Transform::Rotate(float value, glm::vec3 axis)
{
  Rotation += value;

  Rotation.x = fmod(Rotation.x, 360.0f);
  Rotation.y = fmod(Rotation.y, 360.0f);
  Rotation.z = fmod(Rotation.z, 360.0f);

  transform = glm::rotate(transform, glm::radians(value), axis);
}

std::string Transform::Serialize()
{

}