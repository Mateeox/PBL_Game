#include "MapTile.hpp"

MapTile::MapTile(Drawable * aDrawable,std::string aName) : 
 mDrawable(aDrawable),
 mName(aName),
 mSceneNode(SceneNode())
{

    GameObject* gameObj = new GameObject(mSceneNode.local);
    gameObj->AddComponent(mDrawable);
    mSceneNode.AddGameObject(gameObj);
    
}

void MapTile::Draw(glm::mat4 &  transform)
{
    mDrawable->Draw(transform);
}