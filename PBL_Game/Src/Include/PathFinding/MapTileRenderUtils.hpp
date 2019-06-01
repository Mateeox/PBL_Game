#pragma once

#include "Texture.hpp"
#include "MapTile.hpp"
#include "PathFindingUtils.hpp"

#include "SceneNode.hpp"

template <class Graph>
void AssignMapTiles(std::vector<MapTile *> &mapTiles, const Graph &graph, int aMapSize,
                    Texture *freeTex,
                    Texture *pathTex,
                    Texture *SlowerTex,
                    Texture *BlocedTex,
                    Shader &aShaderProgram)
{
    for (int i = 0; i < aMapSize; i++)
    {
        for (int j = 0; j < aMapSize; j++)
        {

            GridLocation id{i, j};
            MapTile *mapTile = new MapTile(i, j, freeTex, aShaderProgram);

            mapTile->AsignTexture(BlocedTex, MapTileProfiles::Blocked);
            mapTile->AsignTexture(pathTex, MapTileProfiles::Path);
            mapTile->AsignTexture(SlowerTex, MapTileProfiles::Slower);

            if (graph.walls.find(id) != graph.walls.end())
            {
                mapTile->SwitchTexture(MapTileProfiles::Blocked);
            }

            mapTiles.push_back(mapTile);
        }
    }
}

template <class Graph>
void ResetMapTilePath(std::vector<MapTile *>& mapTiles,
                      const Graph &graph,
                      int aMapSize,
                      std::vector<GridLocation> *path = nullptr)
{

    for(auto xD :  mapTiles)
    {

        GridLocation* id= new GridLocation{xD->x,xD->y};
     if (graph.walls.find(*id) != graph.walls.end())
      {
        xD->SwitchTexture(MapTileProfiles::Blocked);
      }
       else if (path != nullptr && find(path->begin(), path->end(), *id) != path->end())
      {
        xD->SwitchTexture(MapTileProfiles::Path);
      }
      else if(graph.forests.find(*id) != graph.forests.end() )
      {
          xD->SwitchTexture(MapTileProfiles::Slower);
      }
      else
      {
        xD->SwitchTexture(MapTileProfiles::Basic);
      }

    }
}

static void AddMapTilesToSceneNodes(std::vector<MapTile *> &mapTiles, std::vector<SceneNode *> &sNodes,
                                    GridWithWeights &grid,
                                    Texture *FreeTileTexture,
                                    Texture *PathTileTexture,
                                    Texture *SlowerTileTexture,
                                    Texture *BlockedTileTexture,
                                    Shader &shaderProgram,
                                    const float MapScale, const float floorTransform);
