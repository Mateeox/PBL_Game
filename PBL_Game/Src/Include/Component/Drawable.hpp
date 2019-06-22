#pragma once
#include "Shader.hpp"
#include "Component.hpp"

class Drawable : public ComponentSystem::Component
{
public:
  unsigned int VAO, VBO, EBO;
  Shader & defaultShader;

  Drawable(Shader &aShaderProgram) : defaultShader(aShaderProgram), Component(nullptr)
  {
  }

  virtual void Draw(Shader * ShaderProgram,glm::mat4 &  transform) = 0;

  ComponentSystem::ComponentType GetComponentType() override
  {
    return ComponentSystem::Error;
  }
  
  std::string Serialize() {
	  return Component::Serialize();
  }
};