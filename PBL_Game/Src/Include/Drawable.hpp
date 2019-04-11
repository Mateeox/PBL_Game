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

    ComponentSystem::ComponentType GetComponentType() override
    {
     return ComponentSystem::Error;
    }
	std::string Serialize() override
	{
		return std::to_string(VAO) + ";" + std::to_string(VBO) + ";" + std::to_string(EBO);
	}
};