#pragma once
#include "SceneNode.hpp"
#include "GameObject.hpp"
#include "Component/AnimatedModel.hpp"
#include "Component/Model.hpp"

class KeyDoorFactory
{
public:
static SceneNode * CreateNode(int aNumber,std::string tag, ComponentSystem::Component * aKeyModel);
static std::pair<SceneNode*,SceneNode*> Create(int aNumber, ComponentSystem::Component * aKeyModel,ComponentSystem::Component * aDoorModel, std::vector<SceneNode*>* sNodes);
};