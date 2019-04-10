#include "GameObject.hpp"
#include "Drawable.hpp"
#include "ShapeRenderer3D.hpp"
#include <algorithm>

GameObject::GameObject(Transform & trans):transform(trans)
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

ComponentSystem::Component *GameObject::GetComponent(ComponentSystem::ComponentType type)
{
    using ComponentSystem::ComponentType;
    for (auto comp : components)
    {
        if (comp->GetComponentType() == type )
        {
            return (ShapeRenderer3D*)comp;
        }
    }
    return nullptr;
}

GameObject::GameObject(Drawable *adrawable,Transform & trans):transform(trans)
{
    drawable = adrawable;
}

std::string GameObject::Serialize()
{

}