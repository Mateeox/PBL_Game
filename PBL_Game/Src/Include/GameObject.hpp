#pragma once
#include "Transform.hpp"
#include "Drawable.hpp"
#include <vector>

#include "Component.hpp"

class GameObject
{

public:
    Drawable* drawable;
    std::vector<ComponentSystem::Component*> components;
    GameObject();
    GameObject(Drawable* aDrawable);
    
    ComponentSystem::Component* GetComponent(ComponentSystem::ComponentType type);
    void AddComponent(ComponentSystem::Component*);
    void RemoveComponent(ComponentSystem::ComponentType type);

    void Draw(glm::mat4 transform);
};




// ##### IF hierarhical structure
//GameObject * Parent,
//std::vector<GameObject* Childs>