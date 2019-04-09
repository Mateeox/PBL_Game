#include "GameObject.hpp"
#include "Drawable.hpp"
#include "ShapeRenderer3D.hpp"
#include <algorithm>

GameObject::GameObject()
{

    drawable = nullptr;
}

void GameObject::AddComponent(ComponentSystem::Component* compo)
{
    compo->SetGameObject(this);
    components.push_back(compo);
}


void GameObject::RemoveComponent(ComponentSystem::ComponentType type)
{
    using ComponentSystem::ComponentType;

   auto to_erase = remove_if(components.begin(),components.end(),[type](ComponentSystem::Component* compo)
    {
        return compo->GetComponentType() == type;
    }
    );

    components.erase(to_erase);

}

ComponentSystem::Component *GameObject::GetComponent(ComponentSystem::ComponentType typee)
{
    using ComponentSystem::ComponentType;
    for (auto comp : components)
    {
        if (comp->GetComponentType() == typee )
        {
            return comp;
        }
    }
    return nullptr;
}

GameObject::GameObject(Drawable *adrawable)
{
    drawable = adrawable;
}
void GameObject::Draw(glm::mat4 transform)
{
    if (drawable != nullptr)
    {
        unsigned int transformLoc = glGetUniformLocation(drawable->ShaderProgram.shaderProgramID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        drawable->Draw();
    }
}