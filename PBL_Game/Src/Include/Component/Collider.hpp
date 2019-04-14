#include <glm/glm.hpp>

struct AABB
{
  glm::vec3 position;
  glm::vec3 size;

  inline AABB() : size(1, 1, 1) {}
  inline AABB(const glm::vec3 &o, const glm::vec3 &s) : position(o), size(s) {}
};