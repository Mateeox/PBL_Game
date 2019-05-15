#include "MapTile.hpp"

MapTile::MapTile(ShapeRenderer3D *aDrawable, std::string aName) : mDrawable(aDrawable),
                                                                  mName(aName),
                                                                  mSceneNode(SceneNode()),
                                                                  textureColor(0),
                                                                  x_cord(0),
                                                                  y_cord(0),
                                                                  MapTileStatus(false)
{

    GameObject *gameObj = new GameObject(mSceneNode.local);
    gameObj->AddComponent(mDrawable);
    mSceneNode.AddGameObject(gameObj);
}

void MapTile::SetMapTileStatus(bool state)
{
    MapTileStatus = state;
}

bool MapTile::isBlocked() 
{
    return MapTileStatus;
}

void MapTile::ChangeMapTileColor()
{
    if (textureColor == 0)
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