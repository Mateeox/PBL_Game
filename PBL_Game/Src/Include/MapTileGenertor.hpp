#pragma once
#include "MapTile.hpp"
#include <vector>
#include "ShapeRenderer3D.hpp"
#include "Shader.hpp"

class MapTileGenerator
{
public:
    static std::vector<MapTile *> GetMapInstance(const unsigned weight,
                                                 const unsigned height,
                                                 ShapeRenderer3D &aShapeRenderer3D,
                                                 Shader &ShaderProgram);

    
};