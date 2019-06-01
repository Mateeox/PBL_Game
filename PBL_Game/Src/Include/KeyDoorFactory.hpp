#pragma once
#include "SceneNode.hpp"
#include "GameObject.hpp"
#include "Component/AnimatedModel.hpp"
#include "Component/Model.hpp"

class KeyDoorFactory
{
public:
static std::pair<SceneNode*,SceneNode*>  Create(int aNumber, ComponentSystem::Component * aKeyModel,ComponentSystem::Component * aDoorModel);

};