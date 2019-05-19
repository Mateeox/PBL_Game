
#include "Game.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "Component/Model.hpp"
#include "Component/AnimatedModel.hpp"
#include "Shapes.hpp"
#include "PathFinding/PathFindingUtils.hpp"
#include "PathFinding/MapTile.hpp"

#include <fstream>
#include <iterator>

static bool leftSideActive = true;
static bool swapButtonPressed = false;

void Game::InitializeConfig()
{

  WINDOW_WIDTH = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_WIDTH", ConfigMap);
  WINDOW_HEIGHT = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_HEIGHT", ConfigMap);
  movementSpeed = ConfigUtils::GetValueFromMap<float>("PlayerSpeed", ConfigMap);
}

template <class Graph>
std::vector<MapTile> AssignMapTiles(const Graph &graph, int field_width,
                                    Texture *freeTex,
                                    Texture *pathTex,
                                    Texture *SlowerTex,
                                    Texture *BlocedTex,
                                    Shader &aShaderProgram,
                                    std::vector<GridLocation> *path = nullptr)
{
  std::vector<MapTile> mapTiles;


std::cout<<"Cale Te"<<"\n";
  for (int i = 0; i < field_width; i++)
  {
    for (int j = 0; j < field_width; j++)
    {
      GridLocation id{i, j};
      MapTile mapTile(i, j, freeTex, aShaderProgram);

      if (graph.walls.find(id) != graph.walls.end())
      {

        mapTile.SelectTileProfile(MapTileProfiles::Blocked);
      }

      if (path != nullptr && find(path->begin(), path->end(), id) != path->end())
      {
        mapTile.SelectTileProfile(MapTileProfiles::Path);
      }

      mapTiles.push_back(mapTile);
    }
  }

std::cout<<"Cale Te"<<"\n";
  return mapTiles;
}

Game::Game(Window &aOkno) : okienko(aOkno),
                            camera(Camera()),
                            camera2(Camera())
{

  LoadConfig();
  InitializeConfig();

  glfwSetWindowSize(okienko.window, WINDOW_WIDTH, WINDOW_HEIGHT);
  InitializeConfig();

  shaderProgram = new Shader("Shaders/vertex4.txt", "Shaders/fragment3.txt");
  shaderProgram_For_Model = new Shader("Shaders/vertexModel.txt", "Shaders/fragmentModel.txt");
  shaderAnimatedModel = new Shader("Shaders/skinning.vs", "Shaders/skinning.fs");

  glfwSetCursorPosCallback(okienko.window, mouse_callback);
}

void Game::Granko()
{
  Texture *BlockedTileTexture = new Texture("Textures/BlockedTile.png", GL_LINEAR);
  Texture *FreeTileTexture = new Texture("Textures/FreeTile.png", GL_LINEAR);
  Texture *SlowerTileTexture = new Texture("Textures/SlowerTile.png", GL_LINEAR);
  Texture *PathTileTexture = new Texture("Textures/PathTile.png", GL_LINEAR);

  BlockedTileTexture->Load();
  FreeTileTexture->Load();
  SlowerTileTexture->Load();
  PathTileTexture->Load();

  SceneNode scena1_new;
  SceneNode FloorNode_new;
  SceneNode box1;
  SceneNode box2;
  SceneNode box3;

  SceneNode Enemy_Node;

  GameObject *enemyGameObject = new GameObject(Enemy_Node.local);

  GameObject *leftPlayerObj = new GameObject(leftPlayerNode.local);
  leftPlayerObj->setTag("player");
  GameObject *rightPlayerObj = new GameObject(rightPlayerNode.local);
  rightPlayerObj->setTag("player");

  GameObject *trojObj = new GameObject(scena1_new.world);
  GameObject *FloorObj = new GameObject(FloorNode_new.world);
  GameObject *hexObj2 = new GameObject(box2.local);
  GameObject *hexObj3 = new GameObject(box3.local);

  std::string BeeModelPath = "Models/enemy_model.obj";
  std::string AnimatedEnemyPAth = "Models/" + ConfigUtils::GetValueFromMap<std::string>("Enemy_Animated_Model", ConfigMap);

  Model *BeeModel = new Model(BeeModelPath, *shaderProgram_For_Model, false);
  AnimatedModel *animatedModel = new AnimatedModel(AnimatedEnemyPAth, *shaderAnimatedModel, false);

  ShapeRenderer3D *Floor = new ShapeRenderer3D(Shapes::RainBow_Square,
                                               Shapes::RB_Square_indices,
                                               sizeof(Shapes::RainBow_Square),
                                               sizeof(Shapes::RB_Square_indices),
                                               *shaderProgram,
                                               BlockedTileTexture, "Basic");

  ShapeRenderer3D *TileRenderer = new ShapeRenderer3D(Shapes::RainBow_Square,
                                                      Shapes::RB_Square_indices,
                                                      sizeof(Shapes::RainBow_Square),
                                                      sizeof(Shapes::RB_Square_indices),
                                                      *shaderProgram,
                                                      FreeTileTexture, "Basic");

  ShapeRenderer3D *trojkat = new ShapeRenderer3D(Shapes::RainBow_Triangle,
                                                 Shapes::RB_Triangle_indices,
                                                 sizeof(Shapes::RainBow_Triangle),
                                                 sizeof(Shapes::RB_Triangle_indices),
                                                 *shaderProgram,
                                                 BlockedTileTexture, "Basic");

  ShapeRenderer3D *szescian = new ShapeRenderer3D(Shapes::RainBow_Cube,
                                                  Shapes::RB_Cube_indices,
                                                  sizeof(Shapes::RainBow_Cube),
                                                  sizeof(Shapes::RB_Cube_indices),
                                                  *shaderProgram,
                                                  BlockedTileTexture, "Basic");

  leftPlayerObj->AddComponent(BeeModel);
  rightPlayerObj->AddComponent(BeeModel);
  enemyGameObject->AddComponent(animatedModel);

  trojObj->AddComponent(trojkat);
  FloorObj->AddComponent(Floor);
  hexObj2->AddComponent(szescian);
  hexObj3->AddComponent(szescian);

  Collider *leftPlayerCollider = new Collider(leftPlayerObj->transform);
  Collider *rightPlayerCollider = new Collider(rightPlayerObj->transform);
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

  Enemy_Node.AddGameObject(enemyGameObject);
  scena1_new.AddGameObject(trojObj);
  FloorNode_new.AddGameObject(FloorObj);
  box2.AddGameObject(hexObj2);
  box3.AddGameObject(hexObj3);

  float floorTransform = ConfigUtils::GetValueFromMap<float>("FloorTranslation", ConfigMap);

  float floorTileScale = ConfigUtils::GetValueFromMap<float>("floorTileScale", ConfigMap);

  FloorNode_new.Translate(0, floorTransform, 0);

  leftPlayerNode.Scale(0.01, 0.01, 0.01);
  rightPlayerNode.Scale(0.01, 0.01, 0.01);
  Enemy_Node.Scale(0.01, 0.01, 0.01);
  box1.Scale(0.1f, 0.6f, 1.0f);

  leftPlayerNode.Translate(-150.0, 0, 0);
  rightPlayerNode.Translate(150.0, 0, 0);

  Enemy_Node.Translate(5, 5, 0);
  box2.Translate(5, 0, 0);
  box3.Translate(-5, 0, 0);
  FloorNode_new.Scale(floorTileScale, floorTileScale, floorTileScale);
  FloorNode_new.Rotate(90.0f, glm::vec3(1, 0, 0));

  sNodes.push_back(&Enemy_Node);
  sNodes.push_back(&leftPlayerNode);
  rightNodes.push_back(&rightPlayerNode);
  //sNodes.push_back(&scena1_new);
  sNodes.push_back(&FloorNode_new);

  //sNodes.push_back(&box1);
  sNodes.push_back(&box2);
  sNodes.push_back(&box3);

  GridWithWeights grid = make_diagram4(20, 20);
  GridLocation start{1, 4};
  GridLocation goal{8, 5};
  std::unordered_map<GridLocation, GridLocation> came_from;
  std::unordered_map<GridLocation, double> cost_so_far;
  a_star_search(grid, start, goal, came_from, cost_so_far);
  draw_grid(grid, 2, nullptr, &came_from);
  std::cout << '\n';
  draw_grid(grid, 3, &cost_so_far, nullptr);
  std::cout << '\n';
  std::vector<GridLocation> path = reconstruct_path(start, goal, came_from);
  draw_grid(grid, 3, nullptr, nullptr, &path);

  std::vector<SceneNode *> mapTileSceneNodes;
  std::vector<MapTile> mapTiles = AssignMapTiles(grid,
                                                 20,
                                                 FreeTileTexture,
                                                 PathTileTexture,
                                                 SlowerTileTexture,
                                                 BlockedTileTexture,
                                                 *shaderProgram, &path);

  for (int i = 0; i < 20; i++)
  {
    for (int j = 0; j < 20; j++)
    {
      SceneNode * tileSceneNode = new SceneNode();
      GameObject * gameObject = new GameObject(tileSceneNode->local); 
      gameObject->AddComponent(&mapTiles[i+j]);
  
      sNodes.push_back(tileSceneNode);
    }
  }

  shaderProgram->use();

  uint32 next_game_tick = (glfwGetTime() * 1000);
  int loops;
  float interpolation = 1.0;

  gatherCollidableObjects(sNodes);
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

  delete trojObj;
  delete FloorObj;
  delete hexObj2;
  delete hexObj3;

  delete trojkat;
  delete Floor;
  delete szescian;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glDeleteProgram(shaderProgram->shaderProgramID);
  glfwTerminate();
}

void Game::Update(float interpolation)
{
  if (leftSideActive)
  {
    ProcessInput(interpolation, camera);
  }
  else
  {
    ProcessInput(interpolation, camera2);
  }

  if (leftSideActive)
    UpdatePlayer(leftPlayerNode, camera, interpolation);
  else
    UpdatePlayer(rightPlayerNode, camera2, interpolation);
}

void Game::Render()
{
  glfwPollEvents();
  glScissor(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
  {
    ImGui::Begin("Klawiszologia",
                 &show_demo_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Q - zmiana strony");
    ImGui::Text("Strzalki - ruch postaci");
    ImGui::End();
  }
  ImGui::Render();

  Transform originTransform = Transform::origin();

  SetViewAndPerspective(camera);
  // RENDER LEWEJ STRONY
  glViewport(0, 0, (Game::WINDOW_WIDTH / 2) + 125, Game::WINDOW_HEIGHT);
  glScissor(0, 0, (Game::WINDOW_WIDTH / 2) + offset, Game::WINDOW_HEIGHT);
  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  std::cout << "Thak"
            << "\n";
  for (auto node : sNodes)
  {
    node->Render(originTransform, true);
  }

  SetViewAndPerspective(camera2);

  // RENDER PRAWEJ STRONY
  glViewport((Game::WINDOW_WIDTH / 2) - 125, 0, (Game::WINDOW_WIDTH / 2) + 125, Game::WINDOW_HEIGHT);
  glScissor((Game::WINDOW_WIDTH / 2) + offset, 0, (Game::WINDOW_WIDTH / 2) - offset, Game::WINDOW_HEIGHT);
  glClearColor(0, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  sNodes[2]->Render(originTransform, true);
  sNodes[1]->Render(originTransform, true);
  for (auto node : rightNodes)
  {
    node->Render(originTransform, true);
  }

  // RENDER PASKA ODDZIELAJACAEGO KAMERY - TODO
  glViewport((Game::WINDOW_WIDTH / 2) + offset - 5, 0, 10, Game::WINDOW_HEIGHT);
  glScissor((Game::WINDOW_WIDTH / 2) + offset - 5, 0, 10, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
  this->sNodes[0]->AddChild(this->sNodes[1]);
  this->sNodes[1]->AddParent(this->sNodes[0]);
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

  glm::vec3 move = movementDir * movementSpeed * interpolation;
  player.Translate(move.x, move.y, move.z);
  Collider *playerCollider = ((Collider *)player.gameObject->GetComponent(ComponentSystem::ComponentType::Collider));
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

  camera.Position.x = player.gameObject->transform.getPosition().x * player.gameObject->transform.getScale().x;
  camera.Position.z = player.gameObject->transform.getPosition().z * player.gameObject->transform.getScale().z + cameraZOffset;
}

void Game::gatherCollidableObjects(std::vector<SceneNode *> &nodes)
{
  for (auto node : nodes)
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

std::vector<GameObject*> Game::findByTag(const std::vector<SceneNode *> &data, std::string tag)
{
	std::vector<GameObject*> foundObjects;
	for (auto node : data)
	{
		if (node->gameObject != nullptr && node->gameObject->getTag() == tag)
		{
			foundObjects.push_back(node->gameObject);
		}
		std::vector<GameObject*> foundChildObjects(findByTag(node->children, tag));
		if (foundChildObjects.size() > 0)
		{
			foundObjects.insert(foundObjects.end(), foundChildObjects.begin(), foundChildObjects.end());
		}
	}
	return foundObjects;
}

GameObject* Game::findByTagSingle(const std::vector<SceneNode*>& data, std::string tag)
{
	for (auto node : data)
	{
		if (node->gameObject != nullptr && node->gameObject->getTag() == tag)
		{
			return node->gameObject;
		}
		GameObject* foundChildObject = findByTagSingle(node->children, tag);
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

  shaderAnimatedModel->use();
  shaderAnimatedModel->setMat4("projection", projection);
  shaderAnimatedModel->setMat4("view", view);
}
