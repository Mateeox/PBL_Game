
#include "Game.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "Component/Model.hpp"
#include "Shapes.hpp"
#include "PathFinding/PathFindingUtils.hpp"
#include "KeyDoorFactory.hpp"

#include <fstream>
#include <iterator>

static bool leftSideActive = true;
static bool swapButtonPressed = false;

void Game::InitializeConfig()
{
  MapScale = ConfigUtils::GetValueFromMap<unsigned>("MapScale", ConfigMap);
  WINDOW_WIDTH = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_WIDTH", ConfigMap);
  WINDOW_HEIGHT = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_HEIGHT", ConfigMap);
  movementSpeed = ConfigUtils::GetValueFromMap<float>("PlayerSpeed", ConfigMap);
  debugPathFinding = ConfigUtils::GetValueFromMap<int>("debugPathFinding", ConfigMap);

  EnemyBaseSpeed = ConfigUtils::GetValueFromMap<float>("EnemyBaseSpeed", ConfigMap);
  EnemyXoffset = ConfigUtils::GetValueFromMap<float>("EnemyXoffset", ConfigMap);
  EnemyYoffset = ConfigUtils::GetValueFromMap<float>("EnemyYoffset", ConfigMap);
  EnemyZoffset = ConfigUtils::GetValueFromMap<float>("EnemyZoffset", ConfigMap);

  PlayerXOffset = ConfigUtils::GetValueFromMap<float>("PlayerXOffset", ConfigMap);
  PlayerYOffset = ConfigUtils::GetValueFromMap<float>("PlayerYOffset", ConfigMap);
  PlayerZOffset = ConfigUtils::GetValueFromMap<float>("PlayerZOffset", ConfigMap);

  floorTransform = ConfigUtils::GetValueFromMap<float>("FloorTranslation", ConfigMap);
  TileScale = ConfigUtils::GetValueFromMap<float>("TileScale", ConfigMap);
  EnemyScale = ConfigUtils::GetValueFromMap<float>("EnemyScale", ConfigMap);

  cameraZOffset = ConfigUtils::GetValueFromMap<float>("cameraZOffset", ConfigMap);
  cameraYOffset = ConfigUtils::GetValueFromMap<float>("cameraYOffset", ConfigMap);
  cameraAngle = ConfigUtils::GetValueFromMap<float>("cameraAngle", ConfigMap);

  TileScaleTimes100 = TileScale * 100;
  EnemyScaleInverse = 1 / EnemyScale;

  PlayerScale = ConfigUtils::GetValueFromMap<float>("PlayerScale", ConfigMap);
  PlayerScaleInverse = 1 / PlayerScale;

  movementSpeedTimesPlayerScale = movementSpeed * PlayerScale;
}

Game::Game(Window &aOkno) : okienko(aOkno),
                            camera(Camera()),
                            camera2(Camera()),
                            MapSize(0)
{
  LoadConfig();
  InitializeConfig();

  glfwSetWindowSize(okienko.window, WINDOW_WIDTH, WINDOW_HEIGHT);
  InitializeConfig();

  shaderProgram = new Shader("Shaders/vertex4.txt", "Shaders/fragment3.txt");
  shaderProgram_For_Model = new Shader("Shaders/vertexModel.vs", "Shaders/fragmentModel.fs");
  shaderAnimatedModel = new Shader("Shaders/skinning.vs", "Shaders/skinning.fs");
  shaderViewCone = new Shader("Shaders/viewCone.vs", "Shaders/viewCone.fs");

  glfwSetCursorPosCallback(okienko.window, mouse_callback);
}

void Game::Granko()
{
  MapGenerator generator(shaderProgram_For_Model, MapScale, 3, 4, false);

  std::vector<MapKey *> mapped = generator.GetConverted();

  MapSize = generator.maxSize;
  grid = make_diagramFromGeneratedMap(mapped, MapSize);

  Texture *BlockedTileTexture = new Texture("Textures/BlockedTile.png", GL_LINEAR);
  Texture *FreeTileTexture = new Texture("Textures/FreeTile.png", GL_LINEAR);
  Texture *SlowerTileTexture = new Texture("Textures/SlowerTile.png", GL_LINEAR);
  Texture *PathTileTexture = new Texture("Textures/PathTile.png", GL_LINEAR);

  BlockedTileTexture->Load();
  FreeTileTexture->Load();
  SlowerTileTexture->Load();
  PathTileTexture->Load();

  SceneNode box2;
  SceneNode box3;
  SceneNode doorNode;
  SceneNode keyNode;

  GameObject *enemyGameObject = new GameObject(Enemy_Node_For_Model.local);

  GameObject *leftPlayerObj = new GameObject(leftPlayerNodeForModel.local);
  leftPlayerObj->setTag("player");
  GameObject *rightPlayerObj = new GameObject(rightPlayerNodeForModel.local);
  rightPlayerObj->setTag("player");

  GameObject *hexObj2 = new GameObject(box2.local);
  GameObject *hexObj3 = new GameObject(box3.local);

  GameObject *doorObj = new GameObject(doorNode.local);
  GameObject *keyObj = new GameObject(keyNode.local);

  std::string PlayerModelPath = "Models/Player/Player_Static.obj";
  std::string AnimatedEnemyPAth = "Models/" + ConfigUtils::GetValueFromMap<std::string>("Enemy_Animated_Model", ConfigMap);

  Model *PlayerModel = new Model(PlayerModelPath, *shaderProgram_For_Model, false);
  animatedModel = new AnimatedModel(AnimatedEnemyPAth, *shaderAnimatedModel, false);

  ShapeRenderer3D *TileRenderer = new ShapeRenderer3D(Shapes::RainBow_Square,
                                                      Shapes::RB_Square_indices,
                                                      sizeof(Shapes::RainBow_Square),
                                                      sizeof(Shapes::RB_Square_indices),
                                                      *shaderProgram,
                                                      FreeTileTexture, "Basic");

  ShapeRenderer3D *szescian = new ShapeRenderer3D(Shapes::RainBow_Cube,
                                                  Shapes::RB_Cube_indices,
                                                  sizeof(Shapes::RainBow_Cube),
                                                  sizeof(Shapes::RB_Cube_indices),
                                                  *shaderProgram,
                                                  BlockedTileTexture, "Basic");
  ConeRenderer *coneRendererLeft = new ConeRenderer(*shaderViewCone, &sNodes);

  leftPlayerObj->AddComponent(coneRendererLeft);
  leftPlayerObj->AddComponent(PlayerModel);
  rightPlayerObj->AddComponent(PlayerModel);
  enemyGameObject->AddComponent(animatedModel);

  hexObj2->AddComponent(szescian);
  hexObj3->AddComponent(szescian);

  doorObj->AddComponent(szescian);
  keyObj->AddComponent(szescian);

  Collider *leftPlayerCollider = new Collider(leftPlayerObj->transform);
  Collider *rightPlayerCollider = new Collider(rightPlayerObj->transform);

  // Triggery
  Door *sampleDoor = new Door(doorObj->transform, &doorNode);
  Key *sampleKey = new Key(keyObj->transform, sampleDoor);

  sampleDoor->setDimensions(0, 0, 0, 0.5, 1, 1);
  sampleKey->setDimensions(0, 0, 0, 0.3, 0.3, 0.3);
  doorObj->AddComponent(sampleDoor);
  keyObj->AddComponent(sampleKey);
  doorNode.AddGameObject(doorObj);
  keyNode.AddGameObject(keyObj);

  doorNode.Scale(0.5, 1, 1);
  doorNode.Translate(-30, 0, 0);

  keyNode.Scale(0.3, 0.3, 0.3);
  keyNode.Translate(-80, 0, 0);

  // Koniec triggerow

  leftPlayerCollider->setDimensions(-0.12, 0, 0.25, 2.3, 2, 3.05);
  rightPlayerCollider->setDimensions(-0.12, 0, 0.25, 2.3, 2, 3.05);

  leftPlayerObj->AddComponent(leftPlayerCollider);
  rightPlayerObj->AddComponent(rightPlayerCollider);
  leftPlayerNode.AddGameObject(leftPlayerObj);
  rightPlayerNode.AddGameObject(rightPlayerObj);

  Collider *box2Collider = new Collider(hexObj2->transform);
  box2Collider->setDimensions(0, 0, 0, 2, 2, 2);
  hexObj2->AddComponent(box2Collider);
  Collider *box3Collider = new Collider(hexObj3->transform);
  box3Collider->setDimensions(0, 0, 0, 2, 2, 2);
  hexObj3->AddComponent(box3Collider);

  Enemy_Node_For_Model.AddGameObject(enemyGameObject);
  box2.AddGameObject(hexObj2);
  box3.AddGameObject(hexObj3);

  float floorTransform = ConfigUtils::GetValueFromMap<float>("FloorTranslation", ConfigMap);
  float TileScale = ConfigUtils::GetValueFromMap<float>("TileScale", ConfigMap);

  //  FloorNode_new.Translate(0, floorTransform, 0);

  leftPlayerNode.Scale(PlayerScale);
  rightPlayerNode.Scale(PlayerScale);

  Enemy_Node.Scale(EnemyScale);

  leftPlayerNode.Translate(0, PlayerYOffset, 0);
  rightPlayerNode.Translate(0, PlayerYOffset, 0);

  box2.Translate(5, 0, 0);
  box3.Translate(-5, 0, 0);

  leftPlayerNode.AddChild(&leftPlayerNodeForModel);
  rightPlayerNode.AddChild(&rightPlayerNodeForModel);
  Enemy_Node.AddChild(&Enemy_Node_For_Model);
  sNodes.push_back(&Enemy_Node);

  //sNodes.push_back(&FloorNode_new);
  sNodes.push_back(&box2);
  sNodes.push_back(&box3);

  sNodes.push_back(&doorNode);
  sNodes.push_back(&keyNode);

  glm::vec2 startLocation = FindFirstEmptyFloor(mapped);
  start.x = startLocation.x;
  start.y = startLocation.y;

  Enemy_Node.Translate(start.x * EnemyScaleInverse, EnemyYoffset * 100, start.y * EnemyScaleInverse);
  leftPlayerNode.Translate(start.x * PlayerScaleInverse, 0, start.y * PlayerScaleInverse);

  if (debugPathFinding)
  {
    AddMapTilesToSceneNodes(mapTiles, leftScene,
                            grid,
                            FreeTileTexture,    //Texture 1
                            PathTileTexture,    //Texture 2
                            SlowerTileTexture,  //Texture 3
                            BlockedTileTexture, //Texture 4
                            *shaderProgram,
                            TileScale,
                            floorTransform,
                            MapSize);
  }

  for (auto &node : generator.leftnodes)
  {
    if (node != NULL)
      leftScene.AddChild(node);
  }

  leftScene.AddChild(&leftPlayerNode);
  leftScene.AddChild(&Enemy_Node);

  for (auto &node : generator.rightnodes)
  {
    if (node != NULL)
    {
      rightScene.AddChild(node);
    }
  }
  rightScene.AddChild(&rightPlayerNode);

  shaderProgram->use();

  uint32 next_game_tick = (glfwGetTime() * 1000);
  int loops;
  float interpolation = 1.0;

  sNodes.push_back(&leftPlayerNode);
  rightNodes.push_back(&rightPlayerNode);
  gatherCollidableObjects(sNodes);
  gatherTriggers(sNodes);
  while (glfwGetKey(okienko.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(okienko.window) == 0)
  {
    loops = 0;

    while ((glfwGetTime() * 1000) > next_game_tick && loops < MAX_FRAMESKIP)
    {
      Update(interpolation);

      next_game_tick += SKIP_TICKS;
      loops++;
    }

    interpolation =
        float((glfwGetTime() * 1000) + SKIP_TICKS - next_game_tick) /
        float(SKIP_TICKS);

    Render();
  }

  delete hexObj2;
  delete hexObj3;
  delete szescian;

  ImguiClear();
  glDeleteProgram(shaderProgram->shaderProgramID);
  glfwTerminate();
}

void Game::Update(float interpolation)
{
  if (!inputBlockade)
  {
    if (leftSideActive)
    {
      ProcessInput(interpolation, camera);
    }
    else
    {
      ProcessInput(interpolation, camera2);
    }

    glm::vec2 start_poz = GetPositionOfset(Enemy_Node, MapSize, EnemyXoffset, EnemyZoffset, EnemyScaleInverse);
    start.x = start_poz.x;
    start.y = start_poz.y;

    glm::vec2 end_poz = GetPositionOfset(leftPlayerNode, MapSize, PlayerXOffset, PlayerZOffset, PlayerScaleInverse);
    goal.x = end_poz.x;
    goal.y = end_poz.y;

    ResetMapTilePath(mapTiles, grid, MapSize, &path);

    if (path.size() > 1)
      MoveNodeToMapTile(&Enemy_Node, path[1], interpolation, EnemyBaseSpeed, EnemyXoffset, EnemyZoffset); // TODO Add BaseSpeed

    if (grid.passable(goal))
    {
      a_star_search(grid, start, goal, came_from, cost_so_far);
      path = reconstruct_path(start, goal, came_from);
    }

    if (leftSideActive)
      UpdatePlayer(leftPlayerNode, camera, interpolation);
    else
      UpdatePlayer(rightPlayerNode, camera2, interpolation);
  }
}

void Game::Render()
{
  glfwPollEvents();
  glScissor(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImguiStartEndDraw();

  Transform originTransform = Transform::origin();

  SetViewAndPerspective(camera);
  // RENDER LEWEJ STRONY
  glViewport(0, 0, (Game::WINDOW_WIDTH / 2) + 125, Game::WINDOW_HEIGHT);
  glScissor(0, 0, (Game::WINDOW_WIDTH / 2) + offset, Game::WINDOW_HEIGHT);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  leftScene.Render(originTransform, true);
  Enemy_Node.Render(originTransform, true);

  SetViewAndPerspective(camera2);

  // RENDER PRAWEJ STRONY
  glViewport((Game::WINDOW_WIDTH / 2) - 125, 0, (Game::WINDOW_WIDTH / 2) + 125, Game::WINDOW_HEIGHT);
  glScissor((Game::WINDOW_WIDTH / 2) + offset, 0, (Game::WINDOW_WIDTH / 2) - offset, Game::WINDOW_HEIGHT);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  rightScene.Render(originTransform, true);

  // RENDER PASKA ODDZIELAJACAEGO KAMERY - TODO
  glViewport((Game::WINDOW_WIDTH / 2) + offset - 5, 0, 10, Game::WINDOW_HEIGHT);
  glScissor((Game::WINDOW_WIDTH / 2) + offset - 5, 0, 10, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  // Render grafik
  Plot();

  ImguiDrawData();

  // Swap buffers
  glfwSwapBuffers(okienko.window);
}

void Game::Serialize()
{
  std::map<SceneNode *, unsigned long long> oidMap;
  SerializeFaza1(oidMap);
  std::vector<SceneNode> tempNodes;
  SerializeFaza2(oidMap, tempNodes);
  oidMap.clear();
  SerializeFaza3(tempNodes);
  tempNodes.clear();
}

void Game::SerializeFaza1(std::map<SceneNode *, unsigned long long> &map)
{
  for (SceneNode *scene : this->sNodes)
  {
    unsigned long long n = map.size() + 1;
    map.insert(std::pair<SceneNode *, unsigned long long>(scene, n));
  }
}

void Game::SerializeFaza2(std::map<SceneNode *, unsigned long long> &map, std::vector<SceneNode> &temp)
{
  for (std::pair<SceneNode *, unsigned long long> scene : map)
  {
    SceneNode tempNode;
    tempNode.AddParent((SceneNode *)map[scene.first->parent]);

    if (scene.first->children.size() > 0)
      for (SceneNode *child : scene.first->children)
      {
        if (child)
          tempNode.AddChild((SceneNode *)map[child]);
      }

    if (scene.first->gameObject)
      tempNode.AddGameObject(scene.first->gameObject);

    tempNode.local = scene.first->local;
    tempNode.world = scene.first->world;
    temp.push_back(tempNode);
  }
}

void Game::SerializeFaza3(std::vector<SceneNode> &temp)
{
  std::string serialized = "";
  for (SceneNode node : temp)
  {
    serialized += node.Serialize();
  }
  serialized += "END";
  SerializeZapisz(serialized);
}

void Game::SerializeZapisz(std::string serialized)
{
  std::ofstream out("output.txt");
  out << serialized;
  out.close();
}

void Game::Deserialize(std::string path)
{
  std::ifstream in(path);
  std::map<unsigned long long, SceneNode *> oidMap;
  unsigned long long index = 1;
  std::string line;
  while (std::getline(in, line))
  {
    if (line.substr(0, 2) == "SN")
    {
      SceneNode *node = new SceneNode;
      oidMap.insert(std::pair<unsigned long long, SceneNode *>(index, node));
      index++;
    }

    if (line.substr(0, 3) == "\tW;")
    {
      oidMap[index - 1]->world.Deserialize(line.substr(3));
    }

    if (line.substr(0, 3) == "\tL;")
    {
      oidMap[index - 1]->local.Deserialize(line.substr(3));
    }

    if (line.substr(0, 3) == "\tP;")
    {
      unsigned long long i_dec = atoi(line.substr(3).c_str());
      oidMap[index - 1]->AddParent((SceneNode *)i_dec);
    }

    if (line.substr(0, 4) == "\tCH;")
    {
      unsigned long long i_dec = atoi(line.substr(4).c_str());
      oidMap[index - 1]->AddChild((SceneNode *)i_dec);
    }

    if (line.substr(0, 2) == "\tO")
    {
      Transform transform;
      GameObject go(transform);
      oidMap[index - 1]->AddGameObject(&go);
    }

    if (line.substr(0, 4) == "\t\tT;")
    {
      oidMap[index - 1]->gameObject->transform.Deserialize(line.substr(4));
    }

    if (line.substr(0, 5) == "\t\tCO;")
    {
      line.substr(0, 5); // to be implemented
    }
  }
  in.close();
  DeserializeOrderPointers(oidMap);
}

void Game::DeserializeOrderPointers(std::map<unsigned long long, SceneNode *> &map)
{
  for (std::pair<unsigned long long, SceneNode *> node : map)
  {
    if (node.second->parent > 0 && node.second->parent != node.second)
      node.second->parent = map[(unsigned long long)std::abs((intptr_t)node.second->parent)];
    for (int i = 0; i < node.second->children.size(); i++)
      node.second->children[i] = map[(unsigned long long)std::abs((intptr_t)node.second->children[i])];
    this->sNodes.push_back(node.second);
  }
  map.clear();
}

void Game::SetCamera(Camera aCamera, int camera)
{
  if (camera == 1)
  {
    this->camera = aCamera;
  }
  else
  {
    this->camera2 = aCamera;
  }
}
void Game::ProcessInput(float interpolation, Camera &camera_update)
{

  ProcessMouse();
  if (glfwGetKey(okienko.window, GLFW_KEY_Q) == GLFW_PRESS && swapButtonPressed == false)
  {
    offset *= -1;
    leftSideActive = !leftSideActive;
    swapButtonPressed = true;
  }
  if (swapButtonPressed && glfwGetKey(okienko.window, GLFW_KEY_Q) != GLFW_PRESS)
  {
    swapButtonPressed = false;
  }

  if (!Tab_Pressed)
  {
    if (glfwGetKey(okienko.window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
      Tab_Pressed = true;

      Cursor_Enabled = !Cursor_Enabled;
      if (Cursor_Enabled)
      {
        glfwSetInputMode(okienko.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        glfwSetCursorPos(okienko.window, okienko.iWidth / 2, okienko.iHeight / 2);
      }
      else
      {
        glfwSetInputMode(okienko.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
    }
  }

  if (glfwGetKey(okienko.window, GLFW_KEY_TAB) == GLFW_RELEASE)
  {
    Tab_Pressed = false;
  }
}
void Game::ProcessMouse()
{

  if (mouseCallBack)
  {
    if (mouseToUpdate)
    {
      mouseToUpdate = false;
      if (firstMouse)
      {
        lastX = posx;
        lastY = posy;
        firstMouse = false;
      }

      float xoffset = posx - lastX;
      float yoffset = lastY - posy; // reversed since y-coordinates go from bottom to top

      lastX = posx;
      lastY = posy;

      if (!Cursor_Enabled)
      {
        camera.ProcessMouseMovement(xoffset, yoffset);
      }
    }
  }
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
  mouseToUpdate = true;
  posx = xpos;
  posy = ypos;
}

void Game::UpdatePlayer(SceneNode &player, Camera &camera, float interpolation)
{
  Transform transformBeforeMove(player.gameObject->transform);

  glm::vec3 movementDir(0);

  if (glfwGetKey(okienko.window, GLFW_KEY_W) == GLFW_PRESS)
    movementDir.z = -1;
  if (glfwGetKey(okienko.window, GLFW_KEY_S) == GLFW_PRESS)
    movementDir.z = 1;
  if (glfwGetKey(okienko.window, GLFW_KEY_A) == GLFW_PRESS)
    movementDir.x = -1;
  if (glfwGetKey(okienko.window, GLFW_KEY_D) == GLFW_PRESS)
    movementDir.x = 1;

   if (movementDir.z == -1 && movementDir.x == 0)
     player.local.SetRotation(0, 180, 0);
   else if (movementDir.z == 1 && movementDir.x == 0 )
     player.local.SetRotation(0, 0, 0);
   else if (movementDir.x == -1 && movementDir.y == 0 )
     player.local.SetRotation(0, 90, 0);
   else if (movementDir.x == 1 && movementDir.y == 0 )
     player.local.SetRotation(0, 270, 0);

   
   player.local.Translate(glm::vec3(0, -1.0f * player.local.getPosition().y * PlayerScale, 0));

  glm::vec3 move = movementDir * movementSpeedTimesPlayerScale * interpolation;
  player.Translate(move.x, move.y, move.z);
  Collider *playerCollider = (Collider *)player.gameObject->GetComponent(ComponentSystem::ComponentType::Collider);
  //check if there are any collisions, if yes - abort the move
  for (Collider *collider : collidableObjects)
  {
    if (playerCollider->checkCollision(collider))
    {
      player.local = transformBeforeMove;
      playerCollider->transform = transformBeforeMove;
      break;
    }
  }
  for (Trigger *trigger : triggers)
  {
    if (playerCollider->checkCollision(trigger))
    {
      trigger->ActivateTrigger();
    }
  }

  //view cone
  auto coneRenderer = (ConeRenderer *)player.gameObject->GetComponent(ComponentSystem::ComponentType::ConeRenderer);
  if (coneRenderer != nullptr)
  {
    if (glfwGetKey(okienko.window, GLFW_KEY_LEFT) == GLFW_PRESS)
      coneRenderer->rotateLeft();
    if (glfwGetKey(okienko.window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      coneRenderer->rotateRight();
  }
  camera.Position.x = player.local.getPosition().x * PlayerScale;
  camera.Position.y = cameraYOffset;
  camera.Position.z = player.local.getPosition().z * PlayerScale + cameraZOffset;
}

void Game::gatherCollidableObjects(std::vector<SceneNode *> &nodes)
{
  for (auto node : nodes)
  {
    if (node->gameObject != nullptr)
    {

      if (node->gameObject->getTag() != "player" && node->gameObject->getTag() != "enemy")
      {
        ComponentSystem::Component *possibleCollider = node->gameObject->GetComponent(ComponentSystem::ComponentType::Collider);
        if (possibleCollider != nullptr)
        {
          collidableObjects.push_back((Collider *)possibleCollider);
        }
        gatherCollidableObjects(node->children);
      }
    }
  }
}
void Game::gatherTriggers(std::vector<SceneNode *> &nodes)
{
  for (auto node : nodes)
  {
    if (node->gameObject != nullptr)
    {

      if (node->gameObject->getTag() != "player" && node->gameObject->getTag() != "enemy")
      {
        ComponentSystem::Component *possibleTrigger = node->gameObject->GetComponent(ComponentSystem::ComponentType::Trigger);
        if (possibleTrigger != nullptr)
        {
          triggers.push_back((Trigger *)possibleTrigger);
        }
        gatherTriggers(node->children);
      }
    }
  }
}

std::vector<GameObject *> Game::findByTag(const std::vector<SceneNode *> &data, std::string tag)
{
  std::vector<GameObject *> foundObjects;
  for (auto node : data)
  {
    if (node->gameObject != nullptr && node->gameObject->getTag() == tag)
    {
      foundObjects.push_back(node->gameObject);
    }
    std::vector<GameObject *> foundChildObjects(findByTag(node->children, tag));
    if (foundChildObjects.size() > 0)
    {
      foundObjects.insert(foundObjects.end(), foundChildObjects.begin(), foundChildObjects.end());
    }
  }
  return foundObjects;
}

GameObject *Game::findByTagSingle(const std::vector<SceneNode *> &data, std::string tag)
{
  for (auto node : data)
  {
    if (node->gameObject != nullptr && node->gameObject->getTag() == tag)
    {
      return node->gameObject;
    }
    GameObject *foundChildObject = findByTagSingle(node->children, tag);
    if (foundChildObject != nullptr)
    {
      return foundChildObject;
    }
  }
  return nullptr;
}

void Game::LoadConfig()
{
  tinyxml2::XMLDocument doc;
  doc.LoadFile("Configuration/CaleTe.xml");

  tinyxml2::XMLElement *pRoot = doc.FirstChildElement();

  if (pRoot != nullptr)
  {
    for (tinyxml2::XMLElement *e = pRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
      ConfigUtils::GetVariantValueAndInsertToMap(e, ConfigMap);
    }
  }
}

void Game::SetViewAndPerspective(Camera &aCamera)
{
  projection = glm::perspective(aCamera.Zoom, (float)Game::WINDOW_WIDTH / (float)Game::WINDOW_HEIGHT, 0.1f, 100.0f);
  view = aCamera.GetViewMatrix();

  shaderProgram->use();
  shaderProgram->setMat4("projection", projection);
  shaderProgram->setMat4("view", view);

  shaderProgram_For_Model->use();
  shaderProgram_For_Model->setMat4("projection", projection);
  shaderProgram_For_Model->setMat4("view", view);
  shaderProgram_For_Model->setFloat("FogDensity", FogDensity);
  shaderProgram_For_Model->setFloat("viewSpaceZOffset", cameraZOffset);

  

  shaderViewCone->use();
  shaderViewCone->setMat4("projection", projection);
  shaderViewCone->setMat4("view", view);

  shaderAnimatedModel->use();
  shaderAnimatedModel->setMat4("projection", projection);
  shaderAnimatedModel->setMat4("view", view);
  shaderAnimatedModel->setFloat("FogDensity", FogDensity);
  shaderAnimatedModel->setFloat("viewSpaceZOffset", cameraZOffset);
}

// Funkcje do wyswietlania grafik
void Game::Plot()
{
  if (plotNumber == 0)
  {
    inputBlockade = false;
    return;
  }

  inputBlockade = true;

  static int imageNumber = 1;
  static bool keyPressed = false;
  if (glfwGetKey(okienko.window, GLFW_KEY_ENTER) == GLFW_PRESS && !keyPressed)
  {
    imageNumber++;
    keyPressed = true;
  }
  else if (!glfwGetKey(okienko.window, GLFW_KEY_ENTER) == GLFW_PRESS && keyPressed)
    keyPressed = false;

  switch (plotNumber)
  {
  case 1:
    std::string path = "Textures/1_#.png";

    path[11] = imageNumber + 48;
    if (FILE *file = fopen(path.c_str(), "r"))
    {
      fclose(file);
      DisplayImage(path.c_str(), "Napis");
    }
    else
    {
      imageNumber = 0;
      plotNumber = 0;
    }

    break;
  }
}
void Game::DisplayImage(const char *path, const char *text)
{
  glViewport(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glScissor(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  Texture *imageTex = new Texture(path, GL_NEAREST_MIPMAP_NEAREST);
  imageTex->Load();

  SceneNode imageNode;
  GameObject *imageObj = new GameObject(imageNode.world);

  ShapeRenderer3D *image = new ShapeRenderer3D(
      Shapes::RainBow_Square,
      Shapes::RB_Square_indices,
      sizeof(Shapes::RainBow_Square),
      sizeof(Shapes::RB_Square_indices),
      *shaderProgram_For_Model,
      imageTex, "PlotImage");

  imageObj->AddComponent(image);
  imageNode.AddGameObject(imageObj);

  imageNode.Translate(0.0f, 2.4f, 4.0f);
  imageNode.Rotate(camera.Pitch, glm::vec3(1, 0, 0));
  imageNode.Scale(12.8f / 2.0f, 7.2f / 2.0f, 1);

  Transform originTransform = Transform::origin();
  imageNode.Render(originTransform, true);
}

void Game::MoveNodeToMapTile(SceneNode *sceneNode, GridLocation mapTile, float interpolation, float speed, float NodeXOffset, float NodeZOffset)
{
  glm::vec2 positionA{sceneNode->local.getPosition().x, sceneNode->local.getPosition().z};
  glm::vec2 positionB{NodeXOffset + mapTile.x * EnemyScaleInverse, NodeZOffset + mapTile.y * EnemyScaleInverse};

  glm::vec2 diffVec = positionB - positionA;

  glm::vec3 diffVec3D = {diffVec.x, sceneNode->local.getPosition().y, diffVec.y};

  double veclenght = sqrt(diffVec.x * diffVec.x + diffVec.y * diffVec.y);
  float angle = atan(diffVec3D.y, diffVec3D.x) * 180.0f / 3.14f;

  if (veclenght != 0)
  {
    diffVec.x = diffVec.x / veclenght;
    diffVec.y = diffVec.y / veclenght;
  }
  else
  {
    diffVec = glm::vec2(0, 0);
  }

  vector2DHelper = glm::vec2(positionB.x, positionB.y);
  vector2DHelper2 = glm::vec2(positionA.x, positionA.y);

  float value = interpolation * speed;
  diffVec *= value;

  SceneNode *roationChild = sceneNode->children[0];
  sceneNode->Translate(diffVec.x, 0, diffVec.y);
  if (abs(diffVec.y) > abs(diffVec.x))
  {
    if (diffVec.y < 0)
    {
      roationChild->local.SetRotation(0, 180, 0);
    }
    else
    {
      roationChild->local.SetRotation(0, 0, 0);
    }
  }
  else
  {
    if (diffVec.x < 0)
    {
      roationChild->local.SetRotation(0, 270, 0);
    }
    else
    {
      roationChild->local.SetRotation(0, 90, 0);
    }
  }
}

void Game::ImguiDrawData()
{
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Game::ImguiStartEndDraw()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGuiFunctions();
  ImGui::Render();
}
void Game::ImGuiFunctions()
{

  if (show_demo_window)
  {
    ImGui::Begin("Klawiszologia",
                 &show_demo_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Q - zmiana strony");
    ImGui::Text("Strzalki - ruch postaci");

    if (path.size() > 1)
      ImGui::Text("Next MapTile ID = %i, %i", path[1].x, path[1].y);
    ImGui::Text("Vector to move = %f, %f", vector2DHelper.x, vector2DHelper.y);
    ImGui::Text("Enemy position = %f, %f", Enemy_Node.local.getPosition().x, Enemy_Node.local.getPosition().z);
    ImGui::Text("Player position = %f, %f", leftPlayerNode.local.getPosition().x, leftPlayerNode.local.getPosition().z);
    ImGui::SliderFloat("CameraZOffset", &cameraZOffset, 0, 5);
    ImGui::SliderFloat("CameraYOffset", &cameraYOffset, 0, 5);
    ImGui::SliderFloat("FogDensity", &FogDensity, 0, 1);

    if (ImGui::Button("Printf Path"))
    {
      draw_grid(grid, 3, nullptr, nullptr, &path);
    }
    if (ImGui::Button("List Model AnimationNames"))
    {
      if (animatedModel != nullptr)
        DisplayAnimationInfo(animatedModel);
    }

    if (ImGui::Button("Swtich Enemy Animation"))
    {
      if (animatedModel != nullptr)
      {
        if (animatedModel->GetAnimationNR() == 1)
        {
          animatedModel->SelectAnimation(0);
        }
        else
        {
          animatedModel->SelectAnimation(1);
        }
      }
    }
    if (ImGui::Button("Update Path and use A_Star_Search"))
    {
      if (grid.passable(goal))
      {
        a_star_search(grid, start, goal, came_from, cost_so_far);
        path = reconstruct_path(start, goal, came_from);
      }
      else
      {
        std::cout << "Target not passable"
                  << "\n";
        //TODO find closest pasable
      }
    }

    ImGui::End();
  }
}
void Game::ImguiClear()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Game::DisplayAnimationInfo(AnimatedModel *model)
{
  model->ListAnimationNames();
}

void Game::SetupPlayersColiders()
{
}