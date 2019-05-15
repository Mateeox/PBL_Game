#pragma once
#include "Texture.hpp"
#include "Component/Drawable.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "SceneNode.hpp"
#include <vector>

class MapTile 
{
    
   bool MapTileStatus;
public:
    MapTile(ShapeRenderer3D *aDrawable,std::string name);
    void Draw(glm::mat4 &transform);
    void ChangeMapTileColor();
    int textureColor;
    void SetMapTileStatus(bool state);
    bool isBlocked();
 
    MapTile* PrevoiusMapTile;

    int temporaryCost;
    ShapeRenderer3D * mDrawable;
    std::string mName;
    int x_cord;
    int y_cord;
    SceneNode  mSceneNode;
    std::vector<MapTile*> neighbors;
};