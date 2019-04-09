#pragma once
#include "Shader.hpp"

class Drawable
{
    public:
    unsigned int VAO,VBO,EBO;
    Shader& ShaderProgram;
    Drawable(Shader & aShaderProgram):ShaderProgram(aShaderProgram)
    {
        
    }
    virtual void Draw() = 0;
};