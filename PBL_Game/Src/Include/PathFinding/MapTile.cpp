#include "MapTile.hpp"

MapTile::MapTile(int x_cord, int y_cord, Texture *basicTexture, Shader &aShaderProgram) : x(x_cord), y(y_cord), activeProfile(MapTileProfiles::Basic),
                                                                                          ShapeRenderer3D(Shapes::RainBow_Square,
                                                                                                          Shapes::RB_Square_indices,
                                                                                                          sizeof(Shapes::RainBow_Square),
                                                                                                          sizeof(Shapes::RB_Square_indices),
                                                                                                          aShaderProgram, basicTexture, activeProfile)
{
}

void MapTile::SelectTileProfile(std::string aMapProfile)
{
    activeProfile = aMapProfile;
    this->SwitchTexture(activeProfile);
}
std::string MapTile::GetActiveProfile()
{
    return activeProfile;
}

void MapTile::Draw(glm::mat4 &transform)
{
    ShapeRenderer3D::Draw(transform);
}

ComponentSystem::ComponentType MapTile::GetComponentType()
{
    return ComponentSystem::MapTile;
}