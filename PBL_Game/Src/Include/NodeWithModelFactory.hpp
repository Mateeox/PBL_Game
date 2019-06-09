#pragma once
#include "SceneNode.hpp"
#include "GameObject.hpp"
#include "Component/AnimatedModel.hpp"
#include "Component/Model.hpp"

class NodeWithModelFactory
{
public:
static SceneNode * CreateNode(int aNumber,std::string tag, ComponentSystem::Component * aKeyModel);
};