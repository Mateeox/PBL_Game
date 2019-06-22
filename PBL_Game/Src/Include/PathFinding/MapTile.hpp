#pragma once
#include "Component/ShapeRenderer3D.hpp"
#include "SceneNode.hpp"
#include "Shapes.hpp"

namespace MapTileProfiles
{
 const std::string Basic = "Basic";  //Free
 const std::string Blocked = "Blocked";
 const std::string Path = "Path";
 const std::string Slower = "Slower";
}

class MapTile :public ShapeRenderer3D
{
public:
    MapTile(int x_cord, int y_cord,Texture * basicTexture,Shader & aShaderProgram);
    int x;
    int y;
    
    void SelectTileProfile(std::string mapProfile);
    std::string GetActiveProfile();
    
    void Draw(Shader * shader,glm::mat4 &transform) override;
    ComponentSystem::ComponentType GetComponentType() override;
  

    ~MapTile() = default;
};