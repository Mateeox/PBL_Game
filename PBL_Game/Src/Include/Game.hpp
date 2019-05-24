#pragma once
#include "Window.hpp"
#include "Shader.hpp"
#include "SceneNode.hpp"
#include "Collider.hpp"
#include "Trigger.hpp"
#include "Key.hpp"
#include "Door.hpp"
#include "ConfigUtils.hpp"
#include "PathFinding/MapTile.hpp"
#include "PathFinding/MapTileRenderUtils.cpp"


const int TICKS_PER_SECOND = 32;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

using namespace PBLGame;

static bool mouseToUpdate = false;
static double posx;
static double posy;

static bool First_tab_push = false;
static bool Tab_Pressed = false;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

class Game
{

  Window &okienko;
  Shader *shaderProgram;
  Shader *shaderProgram_For_Model;
  Shader *shaderAnimatedModel;
  std::vector<SceneNode *> sNodes;
  std::vector<SceneNode *> rightNodes;
  
  std::vector<Collider *> collidableObjects;
  std::vector<Trigger *> triggers;

  //PathFinding
  #pragma region PathFindingAndMapGenerationUtils
  std::vector<MapTile *> mapTiles;
  std::vector<GridLocation> path;
  GridWithWeights grid;
  unsigned MapSize;

  GridLocation start{0, 0};
  GridLocation goal{8, 5};
  std::unordered_map<GridLocation, GridLocation> came_from;
  std::unordered_map<GridLocation, double> cost_so_far;

  #pragma endregion PathFindingAndMapGenerationUtils


  SceneNode leftPlayerNode;
  SceneNode rightPlayerNode;

  void LoadConfig();
  void InitializeConfig();
  //How to get value from config
  //ConfigUtils::GetValueFromMap<TYPE>(NAME,ConfigMap) 
  std::map<std::string,VariantType> ConfigMap;
  



  float movementSpeed; //Move to PlayerData

  bool mouseCallBack = true;
  bool firstMouse = true;
  double lastY;
  double lastX;

  //camera
  glm::mat4 view;
  glm::mat4 projection;

  Camera camera;
  Camera camera2;

  //Imgui
  bool show_demo_window = true;
  bool printf_path = false;

  //
  int offset = 125; // Jak bardzo maja sie roznic rozmiary kamery, szerokosc aktywnej to pol okna + offset, szerokosc nieaktywnej to pol okna - offset



public:

  Game(Window &okienko);

  unsigned WINDOW_WIDTH = 0;
  unsigned WINDOW_HEIGHT = 0;

  const float cameraZOffset = 7;
  const float cameraYOffset = 2;
  const float cameraAngle = 35;

  int plotNumber = 1;	// Zmienna wskazujaca na obecna wstawke fabularna
  bool inputBlockade = true;	// Zmienna  blokujaca mozliwosci gracza (domyslnie na czas wstawek fabularnych)

  void SetCamera(Camera camera, int camera_nr);
  void ProcessMouse();
  void ProcessInput(float interpolation, Camera &camera_update);

  void Granko();
  void Update(float interpolation);
  void Render();
  void Serialize();
  void Deserialize(std::string path);
  void UpdatePlayer(SceneNode &player, Camera &camera,float interpolation);
  void gatherCollidableObjects(std::vector<SceneNode *> &nodes);
  void gatherTriggers(std::vector<SceneNode*>& nodes);
  std::vector<GameObject*> findByTag(const std::vector<SceneNode*>& data, std::string tag);
  GameObject * findByTagSingle(const std::vector<SceneNode*>& data, std::string tag);

private:
  void SerializeFaza1(std::map<SceneNode *,unsigned long long> &map);
  void SerializeFaza2(std::map<SceneNode *,unsigned long long> &map, std::vector<SceneNode> &temp);
  void SerializeFaza3(std::vector<SceneNode> &temp);
  void SerializeZapisz(std::string serialized);
  void DeserializeOrderPointers(std::map<unsigned long long, SceneNode *> &map);

  void SetViewAndPerspective(Camera &aCamera);
  void Plot();
  void DisplayImage(const char * path, const char * text);
};
