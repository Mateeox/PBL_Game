#include "KeyDoorFactory.hpp"
#include "Key.hpp"

std::pair<SceneNode *, SceneNode *> KeyDoorFactory::Create(int aNumber, ComponentSystem::Component *aKeyModel, ComponentSystem::Component *aDoorModel, std::vector<SceneNode*>* sNodes)
{
    //Create nodes
    SceneNode *KeyNode = new SceneNode();
    GameObject *KeyGamObj = new GameObject(KeyNode->local);
	KeyNode->gameObject = std::move(KeyGamObj);

    SceneNode *DoorNode = new SceneNode();
    GameObject *DoorGamObj = new GameObject(DoorNode->local);
	DoorNode->gameObject = std::move(DoorGamObj);

    KeyGamObj->setTag("Key" + std::to_string(aNumber));
    KeyGamObj->AddComponent(aKeyModel);

    DoorGamObj->setTag("Door" + std::to_string(aNumber));
    DoorGamObj->AddComponent(aDoorModel);

    //Create triggers (they are connected)
    Key *aKey = new Key(KeyNode->local, DoorNode);

    //Add key and door components to gameobjects
    KeyGamObj->AddComponent(aKey);
	sNodes->push_back(KeyNode);
    return std::pair<SceneNode *, SceneNode *>(KeyNode, DoorNode);
}