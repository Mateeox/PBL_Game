#include "NodeWithModelFactory.hpp"
#include "Key.hpp"

SceneNode * NodeWithModelFactory::CreateNode(int aNumber,std::string tag, ComponentSystem::Component * model)
{

    SceneNode *node = new SceneNode();
    GameObject *gameObj = new GameObject(node->local);
	gameObj->setTag(tag + std::to_string(aNumber));
	gameObj->AddComponent(model);
	node->AddGameObject(std::move(gameObj));

    return std::move(node);

}