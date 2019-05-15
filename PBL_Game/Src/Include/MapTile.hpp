#pragma once
#include "Texture.hpp"
#include "Component/Drawable.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "SceneNode.hpp"
#include <vector>

class MapTile 
{
    
    Drawable * mDrawable;
    

public:
    MapTile(Drawable *aDrawable,std::string name);
    void Draw(glm::mat4 &transform);

    std::string mName;
    SceneNode  mSceneNode;
    std::vector<MapTile*> kids;
};