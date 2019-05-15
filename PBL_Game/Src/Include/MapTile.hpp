#pragma once
#include "Texture.hpp"
#include "Component/Drawable.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "SceneNode.hpp"
#include <vector>

class MapTile 
{
    
   
public:
    MapTile(ShapeRenderer3D *aDrawable,std::string name);
    void Draw(glm::mat4 &transform);
    void ChangeMapTileColor();
    int textureColor;

    ShapeRenderer3D * mDrawable;
    std::string mName;
    SceneNode  mSceneNode;
    std::vector<MapTile*> kids;
};