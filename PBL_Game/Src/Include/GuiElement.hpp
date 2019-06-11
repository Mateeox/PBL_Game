#pragma once
#include <string>
#include <Shader.hpp>
// Include GLEW
#include <GL/gl3w.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace  SimpleGUI
{
   static const  float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
   static const unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


class GuiElement
{
    unsigned int texture1;
    unsigned int VBO, VAO, EBO;
    glm::mat4 transform;

    public:
    GuiElement(std::string texturepath,glm::mat4 aTRansform);
    void Draw(Shader * shader);
    void setTransform(glm::mat4 transform);

};

}