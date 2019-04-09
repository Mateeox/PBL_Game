#include "GameObject.hpp"
#include "Drawable.hpp"

GameObject::GameObject()
{
    
    drawable = nullptr;
}
GameObject::GameObject(Drawable* adrawable)
{
    drawable = adrawable;
}
void GameObject::Draw(glm::mat4 transform)
{
    if(drawable != nullptr)
    {
      unsigned int transformLoc = glGetUniformLocation(drawable->ShaderProgram.shaderProgramID, "transform");
      glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
      drawable->Draw();
    }
}