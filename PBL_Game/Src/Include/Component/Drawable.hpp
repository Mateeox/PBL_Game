#pragma once
#include "Shader.hpp"
#include "Component.hpp"

class Drawable : public ComponentSystem::Component
{
public:
  unsigned int VAO, VBO, EBO;
  Shader &ShaderProgram;

  Drawable(Shader &aShaderProgram) : ShaderProgram(aShaderProgram), Component(nullptr)
  {
  }

  virtual void Draw(glm::mat4 &  transform) = 0;

  ComponentSystem::ComponentType GetComponentType() override
  {
    return ComponentSystem::Error;
  }
  
  std::string Serialize() {
	  return Component::Serialize();
  }
};