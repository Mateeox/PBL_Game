/*
 Sample code from https://www.redblobgames.com/pathfinding/a-star/
 Copyright 2014 Red Blob Games <redblobgames@gmail.com>
 
 Feel free to use this code in your own projects, including commercial projects
 License: Apache v2.0 <http://www.apache.org/licenses/LICENSE-2.0.html>
*/

#include "PathFindingUtils.hpp"

std::array<GridLocation, 4> SquareGrid::DIRS =
    {GridLocation{1, 0}, GridLocation{0, -1}, GridLocation{-1, 0}, GridLocation{0, 1}};

glm::vec2 GetPositionOfset(SceneNode &node, int maxMapSize, float X_offSet, float Z_offset)
{

  int x = (X_offSet + (int)node.local.getPosition().x) * node.local.getScale().x;
  if (x < 0)
  {
    x = 0;
  }
  if (x >= maxMapSize)
  {
    x = maxMapSize - 1;
  }
  int z = (Z_offset + (int)node.local.getPosition().z) * node.local.getScale().z;

  if (z <= 0)
  {
    z = 0;
  }

  if (z > maxMapSize)
  {
    z = maxMapSize - 1;
  }

  return glm::vec2(x, z);
}

glm::vec2 FindFirstEmptyFloor(std::vector<MapKey *> &map)
{
  for (auto pair : map)
  {
    if (pair->type == MapType::Floor)
    {
      std::cout << "x:" << pair->x << "y:" << pair->y << "\n";
      return glm::vec2(pair->x, pair->y);
    }
  }
}

glm::vec2 FindFirstFromLeftUpCorner(std::vector<MapKey *> &map, int size)
{
  bool firstTile = true;
  glm::vec2 position{ 0, 0 };
  for (auto mapKey : map)
  {

    if (mapKey->type == MapType::Floor && firstTile)
    {
      position = {mapKey->x, mapKey->y};
      firstTile = false;
    }
    else
    {
      if (mapKey->x <= position.x && mapKey->y <= position.y)
      {
        position = {mapKey->x, mapKey->y};
      }
    }
  }
  return position;
}
glm::vec2 FindFirstFromLeftDownCorner(std::vector<MapKey *> &map, int size)
{
  bool firstTile = true;
  glm::vec2 position{ 0, 0 };
  for (auto mapKey : map)
  {
    
    if (mapKey->type == MapType::Floor && firstTile)
    {
      position = {mapKey->x, mapKey->y};
      firstTile = false;
    }
    else
    {
      if (mapKey->x <= position.x && mapKey->y >= position.y)
      {
        position = {mapKey->x, mapKey->y};
      }
    }
  }
  return position;
}
glm::vec2 FindFirstFromRightUpCorner(std::vector<MapKey *> &map, int size)
{
  bool firstTile = true;
  glm::vec2 position{ 0, 0 };
  for (auto mapKey : map)
  {
    
    if (mapKey->type == MapType::Floor && firstTile)
    {
      position = {mapKey->x, mapKey->y};
      firstTile = false;
    }
    else
    {
      if (mapKey->x >= position.x && mapKey->y <= position.y)
      {
        position = {mapKey->x, mapKey->y};
      }
    }
  }
  return position;
}
glm::vec2 FindFirstFromRightDownCorner(std::vector<MapKey *> &map, int size)
{
  bool firstTile = true;
  glm::vec2 position{ 0, 0 };
  for (auto mapKey : map)
  {
    
    if (mapKey->type == MapType::Floor && firstTile)
    {
      position = {mapKey->x, mapKey->y};
      firstTile = false;
    }
    else
    {
      if (mapKey->x >= position.x && mapKey->y >= position.y)
      {
        position = {mapKey->x, mapKey->y};
      }
    }
  }
  return position;
}