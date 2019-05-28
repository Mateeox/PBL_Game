/*
 Sample code from https://www.redblobgames.com/pathfinding/a-star/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 
 Feel free to use this code in your own projects, including commercial projects
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

#include "PathFindingUtils.hpp"

std::array<GridLocation, 4> SquareGrid::DIRS =
    {GridLocation{1, 0}, GridLocation{0, -1}, GridLocation{-1, 0}, GridLocation{0, 1}};

glm::vec2 GetPositionOfset(SceneNode& node,int maxMapSize,float X_offSet,float Z_offset,float mapScale)
{

      int x = (X_offSet + (int)node.local.getPosition().x) / mapScale;
    if (x < 0)
    {
      x = 0;
    }
    if (x >= maxMapSize)
    {
      x = maxMapSize-1;
    }
    int z = (Z_offset + (int)node.local.getPosition().z) / mapScale;

    if (z <= 0)
    {
      z = 0;
    }

    if (z > maxMapSize)
    {
      z = maxMapSize-1;
    }

    return glm::vec2(x,z);
  
}
