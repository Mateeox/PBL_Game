#include "MapTileRenderUtils.hpp"

static void AddMapTilesToSceneNodes(std::vector<SceneNode *> &sNodes, GridWithWeights &grid,
                             Texture *FreeTileTexture, Texture *PathTileTexture, Texture *SlowerTileTexture, Texture *BlockedTileTexture,
                             Shader &shaderProgram, std::vector<GridLocation> &path,const float MapScale,const float floorTransform)
{

 std::vector<MapTile *> mapTiles;

  AssignMapTiles(mapTiles, grid,
                 20,
                 FreeTileTexture,
                 PathTileTexture,
                 SlowerTileTexture,
                 BlockedTileTexture,
                 shaderProgram, &path);

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
    sNodes.push_back(std::move(tileSceneNode));
  }
}