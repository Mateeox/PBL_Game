#pragma once
#include "Texture.hpp"
#include "Drawable.hpp"
#include "ShapeRenderer3D.hpp"
#include <vector>

class MapTile : public Drawable
{
    ShapeRenderer3D &mShapeRenderer3D;
    std::string mName;

public:
    MapTile(ShapeRenderer3D &aShapeRenderer3D, Shader &ShaderProgram,std::string name);
    void Draw(glm::mat4 &transform) override;

    std::vector<MapTile*> kids;
};