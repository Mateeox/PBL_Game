#pragma once
#include "MapTile.hpp"
#include <vector>
#include "Component/Drawable.hpp"
#include "Shader.hpp"

class MapTileUtils
{
public:
    static std::vector<MapTile *> GetMapInstance(const unsigned weight,
                                                 const unsigned height,
                                                 Drawable *aDrawable);

    std::vector<MapTile *> FindPath(MapTile*,MapTile*);
    

};