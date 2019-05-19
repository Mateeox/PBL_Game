#include "Component/ShapeRenderer3D.hpp"
#include "SceneNode.hpp"
#include "Shapes.hpp"


//map TileProfiles

namespace MapTileProfiles
{
 const std::string Basic = "Basic";  //Free
 const std::string Blocked = "Blocked";
 const std::string Path = "Path";
 const std::string Slower = "Slower";

}

class MapTile :public ShapeRenderer3D
{
    std::string activeProfile;
public:
      MapTile(int x_cord, int y_cord,Texture * basicTexture,Shader & aShaderProgram);

    int x;
    int y;
    
    void SelectTileProfile(std::string mapProfile);
     std::string GetActiveProfile();
    
    void Draw(glm::mat4 &transform) override;
    ComponentSystem::ComponentType GetComponentType() override;
  

    ~MapTile() = default;
};