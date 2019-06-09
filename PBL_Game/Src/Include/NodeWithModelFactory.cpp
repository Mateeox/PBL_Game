#include "NodeWithModelFactory.hpp"
#include "Key.hpp"

SceneNode * NodeWithModelFactory::CreateNode(int aNumber,std::string tag, ComponentSystem::Component * model)
{

    SceneNode *node = new SceneNode();
    GameObject *gameObj = new GameObject(node->local);
	node->gameObject = std::move(gameObj);

    gameObj->setTag(tag + std::to_string(aNumber));
    gameObj->AddComponent(model);

    return std::move(node);

}