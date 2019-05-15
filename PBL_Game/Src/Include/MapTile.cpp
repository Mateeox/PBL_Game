#include "MapTile.hpp"

MapTile::MapTile(ShapeRenderer3D *aDrawable, std::string aName) : mDrawable(aDrawable),
                                                                  mName(aName),
                                                                  mSceneNode(SceneNode()),
                                                                  textureColor(0)
{

    GameObject *gameObj = new GameObject(mSceneNode.local);
    gameObj->AddComponent(mDrawable);
    mSceneNode.AddGameObject(gameObj);
}

void MapTile::ChangeMapTileColor()
{
    if(textureColor == 0 )
    {
         mDrawable->SwitchTexture(1);
         textureColor = 1;
    }
    else
    {
      mDrawable->SwitchTexture(0);
      textureColor = 0;
    }
   
}

void MapTile::Draw(glm::mat4 &transform)
{
    mDrawable->Draw(transform);
}