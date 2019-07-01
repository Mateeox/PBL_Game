#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <vector>

// Include GLEW
#include <GL/gl3w.h>
class Transform
{

  glm::mat4 transform;
  glm::vec3 Position;
  glm::vec3 Scale;
  glm::vec3 Rotation;
  glm::mat4 Rotation_matrix;
  glm::quat Quaterion;

public:
  Transform();

  void ScaleTransform(GLfloat x, GLfloat y, GLfloat z);
  void SetTransform(glm::mat4 transform);
  void SetRotation(float x, float y, float z);
  void SetPosition(float x, float y, float z);
  void SetPosition(glm::vec3 vector);
  void SetScale(float x, float y, float z);

  static  Transform origin();

  Transform combine(Transform &other);
  void Translate(glm::vec3 Value);
  void Rotate(float value, glm::vec3 axis);
  glm::mat4 & GetTransform();
  
  std::string Serialize();
  void Deserialize(std::string data);

  glm::mat4  GetTransformCopy();
  glm::vec3 getPosition();
  glm::vec3 getScale();
};
