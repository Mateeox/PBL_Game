#pragma once
#include "Transform.hpp"
#include "Drawable.hpp"
#include <vector>

class GameObject
{

public:
    Drawable* drawable;

    GameObject();
    GameObject(Drawable* aDrawable);
    void Draw(glm::mat4 transform);
};




// ##### IF hierarhical structure
//GameObject * Parent,
//std::vector<GameObject* Childs>