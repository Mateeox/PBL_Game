#include "KeyDoorFactory.hpp"
#include "Key.hpp"
#include "Door.hpp"

std::pair<SceneNode *, SceneNode *> KeyDoorFactory::Create(int aNumber, ComponentSystem::Component *aKeyModel, ComponentSystem::Component *aDoorModel)
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
    Door *aDoor = new Door(DoorNode->local, DoorNode);
    Key *aKey = new Key(KeyNode->local, aDoor);

    //Add key and door components to gameobjects
    KeyGamObj->AddComponent(aKey);
    DoorGamObj->AddComponent(aDoor);

    return std::pair<SceneNode *, SceneNode *>(KeyNode, DoorNode);
}