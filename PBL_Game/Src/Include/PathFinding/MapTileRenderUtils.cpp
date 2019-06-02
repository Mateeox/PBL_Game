#include "MapTileRenderUtils.hpp"

static void AddMapTilesToSceneNodes(std::vector<MapTile *>& mapTiles,SceneNode& sNodes, GridWithWeights &grid,
                             Texture *FreeTileTexture, Texture *PathTileTexture, Texture *SlowerTileTexture, Texture *BlockedTileTexture,
                             Shader &shaderProgram,
                              const float MapScale,
                              const float floorTransform,
                              const float MapSize)
{
  AssignMapTiles(mapTiles, grid,
                 MapSize,
                 FreeTileTexture,
                 PathTileTexture,
                 SlowerTileTexture,
                 BlockedTileTexture,
                 shaderProgram);

  for (auto xd : mapTiles)
  {
    SceneNode *tileSceneNode = new SceneNode();
    GameObject *gameObject = new GameObject(tileSceneNode->local);
    gameObject->AddComponent(xd);
    tileSceneNode->AddGameObject(gameObject);

    tileSceneNode->Translate((MapScale)*xd->x, floorTransform, (MapScale)*xd->y);
    tileSceneNode->Scale(MapScale, 1, MapScale);
    tileSceneNode->Rotate(90, glm::vec3(1, 0, 0));
    gameObject->setTag("MapTile");
    sNodes.AddChild(std::move(tileSceneNode));
   // sNodes.push_back(tileSceneNode);
  }
}