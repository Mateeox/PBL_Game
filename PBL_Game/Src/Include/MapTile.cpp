#include "MapTile.hpp"

MapTile::MapTile(ShapeRenderer3D &aShapeRender, Shader &aShaderProgram,std::string aName) : 
mShapeRenderer3D(aShapeRender),
 Drawable(aShaderProgram),
 mName(aName)
{

}

void MapTile::Draw(glm::mat4 &  transform)
{
    mShapeRenderer3D.Draw(transform);
}