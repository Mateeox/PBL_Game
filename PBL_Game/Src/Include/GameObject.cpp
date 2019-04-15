#include "GameObject.hpp"
#include "Component/Drawable.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include <algorithm>

GameObject::GameObject(Transform &trans) : transform(trans) {}

void GameObject::AddComponent(ComponentSystem::Component *compo)
{
    compo->SetGameObject(this);
    components.push_back(compo);
}

void GameObject::RemoveComponent(ComponentSystem::ComponentType type)
{
    using ComponentSystem::ComponentType;

    auto to_erase = remove_if(components.begin(), components.end(), [type](ComponentSystem::Component *compo) {
        return compo->GetComponentType() == type;
    });

    components.erase(to_erase);
}

ComponentSystem::Component *GameObject::GetComponent(ComponentSystem::ComponentType type)
{
    using ComponentSystem::ComponentType;
    for (auto comp : components)
    {
        if (comp->GetComponentType() == type)
        {
            return comp;
        }
    }
    return nullptr;
}

std::string GameObject::Serialize()
{
	std::string str = "";
	//str += "\tD;" + drawable->Serialize() + "\n\t\t";
	str += "T;" + transform.Serialize();
	for (ComponentSystem::Component* component : components)
	{
		str += "\n\t\tCO;" + component->Serialize();
	}
	return str;
}
