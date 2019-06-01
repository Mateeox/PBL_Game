#pragma once
#include "SceneNode.hpp"
#include "GameObject.hpp"
#include "Component/AnimatedModel.hpp"
#include "Component/Model.hpp"

class KeyDoorFactory
{

static std::pair<SceneNode*,SceneNode*>  Create(int aNumber, std::string aTag, ComponentSystem::Component * aKeyModel,ComponentSystem::Component * aDoorModel);

};