#pragma once
#include "MapTile.hpp"
#include <vector>
#include "Component/ShapeRenderer3D.hpp"
#include "Shader.hpp"


class MapTileUtils
{
public:
    static MapTile *** GetMapInstance(const int weight,
                                                 const int height,
                                                 ShapeRenderer3D *aDrawable,
                                                 float MapScale,
                                                 float FloorTranslation);

    static std::vector<MapTile *> FindPath(MapTile ***map,MapTile* aStart,MapTile* aEnd,const int m,const int n);
    
    static  MapTile* GetClosestMapTile(std::vector<MapTile *>  children,MapTile ***map,int target_x,int target_y);
};

//help notes 
// https://www.redblobgames.com/pathfinding/a-star/introduction.html