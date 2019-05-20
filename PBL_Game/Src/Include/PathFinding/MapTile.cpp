#include "MapTile.hpp"

MapTile::MapTile(int x_cord, int y_cord, Texture *basicTexture, Shader &aShaderProgram) : x(x_cord), y(y_cord),
                                                                                          ShapeRenderer3D(Shapes::RainBow_Square,
                                                                                                          Shapes::RB_Square_indices,
                                                                                                          sizeof(Shapes::RainBow_Square),
                                                                                                          sizeof(Shapes::RB_Square_indices),
                                                                                                          aShaderProgram, basicTexture, MapTileProfiles::Basic)
{
    textures[MapTileProfiles::Basic] = basicTexture;
}

void MapTile::SelectTileProfile(std::string aMapProfile)
{
    textureDisplayed = aMapProfile;
}
std::string MapTile::GetActiveProfile()
{
    return textureDisplayed;
}

void MapTile::Draw(glm::mat4 &transform)
{
    ShapeRenderer3D::Draw(transform);
}

ComponentSystem::ComponentType MapTile::GetComponentType()
{
    return ComponentSystem::MapTile;
}