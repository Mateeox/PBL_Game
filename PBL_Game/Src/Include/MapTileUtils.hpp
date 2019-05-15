#pragma once
#include "MapTile.hpp"
#include <vector>
#include "Component/ShapeRenderer3D.hpp"
#include "Shader.hpp"


class MapTileUtils
{
public:
    static std::vector<MapTile *> GetMapInstance(const unsigned weight,
                                                 const unsigned height,
                                                 ShapeRenderer3D *aDrawable,
                                                 float MapScale,
                                                 float FloorTranslation);

    static std::vector<MapTile *> FindPath(MapTile* aStart,MapTile* aEnd,int m,int n);
    

};

//help notes 
// https://www.redblobgames.com/pathfinding/a-star/introduction.html