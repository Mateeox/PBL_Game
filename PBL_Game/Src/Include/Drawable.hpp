#pragma once
#include "Shader.hpp"
#include "Component.hpp"

class Drawable : public ComponentSystem::Component
{
  public:
    unsigned int VAO, VBO, EBO;
    Shader &ShaderProgram;
    Drawable(Shader &aShaderProgram) : ShaderProgram(aShaderProgram)
    {
    }

    ComponentSystem::ComponentType GetComponentType() override
    {
     return ComponentSystem::Error;
    }
};