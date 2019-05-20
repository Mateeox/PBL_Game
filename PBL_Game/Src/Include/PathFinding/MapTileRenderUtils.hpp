#pragma once

#include "Texture.hpp"
#include "MapTile.hpp"
#include "PathFindingUtils.hpp"

#include "SceneNode.hpp"

template <class Graph>
void AssignMapTiles(std::vector<MapTile *> &mapTiles, const Graph &graph, int field_width,
                    Texture *freeTex,
                    Texture *pathTex,
                    Texture *SlowerTex,
                    Texture *BlocedTex,
                    Shader &aShaderProgram,
                    std::vector<GridLocation> *path = nullptr)
{
    for (int i = 0; i < field_width; i++)
    {
        for (int j = 0; j < field_width; j++)
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

            if (path != nullptr && find(path->begin(), path->end(), id) != path->end())
            {
                mapTile->SwitchTexture(MapTileProfiles::Path);
            }

            mapTiles.push_back(std::move(mapTile));
        }
    }
}

template <class Graph>
void ResetMapTilePath(std::vector<MapTile *> &mapTiles,
                    const Graph &graph,
                    int field_width,
                    std::vector<GridLocation> *path = nullptr)
{

    for (int i = 0; i < field_width; i++)
    {
        for (int j = 0; j < field_width; j++)
        {
            GridLocation id{i, j};

            if (path != nullptr && find(path->begin(), path->end(), id) != path->end())
            {
                mapTiles[i+i*j]->SwitchTexture(MapTileProfiles::Path);
                continue;
            }

            mapTiles[i+i*j]->SwitchTexture(MapTileProfiles::Basic);
        }
    }
}

static void AddMapTilesToSceneNodes(std::vector<MapTile *> mapTiles, std::vector<SceneNode *> &sNodes,
                                    GridWithWeights &grid,
                                    Texture *FreeTileTexture,
                                    Texture *PathTileTexture,
                                    Texture *SlowerTileTexture,
                                    Texture *BlockedTileTexture,
                                    Shader &shaderProgram,
                                    std::vector<GridLocation> &path,
                                    const float MapScale, const float floorTransform);
