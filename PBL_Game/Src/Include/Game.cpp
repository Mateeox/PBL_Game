
#include "Game.hpp"
#include "Component/ShapeRenderer3D.hpp"
#include "Component/Model.hpp"
#include "Shapes.hpp"
#include "PathFinding/PathFindingUtils.hpp"
#include "NodeWithModelFactory.hpp"
#include "EnemyKills.hpp"
#include <random>
#include <chrono> 

#include <fstream>
#include <iterator>

static bool leftSideActive = true;
static bool swapButtonPressed = false;

void Game::InitializeConfig()
{
   using namespace ConfigUtils;
  MapScale = GetValueFromMap<unsigned>("MapScale", GlobalConfigMap);
  WINDOW_WIDTH = GetValueFromMap<unsigned>("WINDOW_WIDTH", GlobalConfigMap);
  WINDOW_HEIGHT = GetValueFromMap<unsigned>("WINDOW_HEIGHT", GlobalConfigMap);
  movementSpeed = GetValueFromMap<float>("PlayerSpeed", GlobalConfigMap);
  debugPathFinding = GetValueFromMap<int>("debugPathFinding", GlobalConfigMap);

  EnemyBaseSpeed = GetValueFromMap<float>("EnemyBaseSpeed", GlobalConfigMap);
  EnemyXoffset = GetValueFromMap<float>("EnemyXoffset", GlobalConfigMap);
  EnemyYoffset = GetValueFromMap<float>("EnemyYoffset", GlobalConfigMap);
  EnemyZoffset = GetValueFromMap<float>("EnemyZoffset", GlobalConfigMap);

  PlayerXOffset = GetValueFromMap<float>("PlayerXOffset", GlobalConfigMap);
  PlayerYOffset = GetValueFromMap<float>("PlayerYOffset", GlobalConfigMap);
  PlayerZOffset = GetValueFromMap<float>("PlayerZOffset", GlobalConfigMap);

  floorTransform = GetValueFromMap<float>("FloorTranslation", GlobalConfigMap);
  TileScale = GetValueFromMap<float>("TileScale", GlobalConfigMap);
  EnemyScale = GetValueFromMap<float>("EnemyScale", GlobalConfigMap);

  cameraZOffset = GetValueFromMap<float>("cameraZOffset", GlobalConfigMap);
  cameraYOffset = GetValueFromMap<float>("cameraYOffset", GlobalConfigMap);
  cameraAngle = GetValueFromMap<float>("cameraAngle", GlobalConfigMap);

  camera.Pitch = GetValueFromMap<float>("cameraPitch", GlobalConfigMap);
  camera.Yaw = GetValueFromMap<float>("cameraYaw", GlobalConfigMap);

  camera2.Pitch = GetValueFromMap<float>("cameraPitch", GlobalConfigMap);
  camera2.Yaw = GetValueFromMap<float>("cameraYaw", GlobalConfigMap);

  TrapScale = GetValueFromMap<float>("TrapScale", GlobalConfigMap);

  TileScaleTimes100 = TileScale * 100;
  EnemyScaleInverse = 1 / EnemyScale;

  PlayerScale = GetValueFromMap<float>("PlayerScale", GlobalConfigMap);
  PlayerScaleInverse = 1 / PlayerScale;

  movementSpeedTimesPlayerScale = movementSpeed * PlayerScale;
}

Game::Game(Window &aOkno) : okienko(aOkno),
                            camera(Camera()),
                            camera2(Camera()),
                            MapSize(0),
							enemyController(nullptr)
{
  InitializeConfig();

  glfwSetWindowSize(okienko.window, WINDOW_WIDTH, WINDOW_HEIGHT);
  InitializeConfig();

  shaderProgram = new Shader("Shaders/vertex4.txt", "Shaders/fragment3.txt");
  shaderProgram_For_Model = new Shader("Shaders/vertexModel.vs", "Shaders/fragmentModel.fs");
  shaderAnimatedModel = new Shader("Shaders/skinning.vs", "Shaders/skinning.fs");
  shaderViewCone = new Shader("Shaders/viewCone.vs", "Shaders/viewCone.fs");

  shaderProgram_For_Model->use();
  shaderProgram_For_Model->setVec3("material.ambient", 0.1,0.1,0.1);
  shaderProgram_For_Model->setVec3("material.diffuse", 0.9,0.9,0.9);
  shaderProgram_For_Model->setVec3("material.specular", 0.1,0.1,0.1);
  shaderProgram_For_Model->setFloat("material.shininess", 2.0f);

  //player
  shaderProgram_For_Model->setVec3("pointLights[0].ambient", 1, 1, 1);
  shaderProgram_For_Model->setVec3("pointLights[0].diffuse", 1, 1, 1);
  shaderProgram_For_Model->setVec3("pointLights[0].specular", 1, 1, 1);
  shaderProgram_For_Model->setFloat("pointLights[0].constant", 1.0f);
  shaderProgram_For_Model->setFloat("pointLights[0].linear", 0.35);
  shaderProgram_For_Model->setFloat("pointLights[0].quadratic", 0.44);

  //enemy
  shaderProgram_For_Model->setVec3("pointLights[1].ambient", 1, 1, 1);
  shaderProgram_For_Model->setVec3("pointLights[1].diffuse", 1, 1, 1);
  shaderProgram_For_Model->setVec3("pointLights[1].specular", 1, 1, 1);
  shaderProgram_For_Model->setFloat("pointLights[1].constant", 1.0f);
  shaderProgram_For_Model->setFloat("pointLights[1].linear", 0.14);
  shaderProgram_For_Model->setFloat("pointLights[1].quadratic", 0.1);

  glfwSetCursorPosCallback(okienko.window, mouse_callback);
}

void Game::Granko()
{
  MapGenerator generator(shaderProgram_For_Model, MapScale, 10, 4, false, &sNodes);

  std::vector<MapKey *> mapped = generator.GetConverted();

  MapSize = generator.maxSize;
  grid = make_diagramFromGeneratedMap(mapped, MapSize);

  glm::vec2 leftDown = FindFirstFromLeftDownCorner(mapped, MapSize);
  glm::vec2 rightDown = FindFirstFromRightDownCorner(mapped, MapSize);
  glm::vec2 leftUp = FindFirstFromLeftUpCorner(mapped, MapSize);
  glm::vec2 rightUp = FindFirstFromRightUpCorner(mapped, MapSize);

  std::cout << " leftDown "
            << "x: " << leftDown.x << " y: " << leftDown.y << "\n";
  std::cout << " rightDown "
            << "x: " << rightDown.x << " y: " << rightDown.y << "\n";
  std::cout << " leftUp "
            << "x: " << leftUp.x << " y: " << leftUp.y << "\n";
  std::cout << " rightUp"
            << "x: " << rightUp.x << " y: " << rightUp.y << "\n";

  Corners = { leftDown,rightDown,leftUp,rightUp };
  srand(time(0));
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(Corners.begin(), Corners.end(), std::default_random_engine(seed));


  Texture *BlockedTileTexture = new Texture("Textures/BlockedTile.png", GL_LINEAR);
  Texture *FreeTileTexture = new Texture("Textures/FreeTile.png", GL_LINEAR);
  Texture *SlowerTileTexture = new Texture("Textures/SlowerTile.png", GL_LINEAR);
  Texture *PathTileTexture = new Texture("Textures/PathTile.png", GL_LINEAR);

  BlockedTileTexture->Load();
  FreeTileTexture->Load();
  SlowerTileTexture->Load();
  PathTileTexture->Load();


  GameObject *enemyGameObject = new GameObject(Enemy_Node_For_Model.local);

  GameObject *leftPlayerObj = new GameObject(leftPlayerNodeForModel.local);
  GameObject *rightPlayerObj = new GameObject(rightPlayerNodeForModel.local);

  GameObject *leftPlayerObjWithCollider = new GameObject(leftPlayerNode.local);
  GameObject *rightPlayerObjWithCollider = new GameObject(rightPlayerNode.local);
  leftPlayerObjWithCollider->setTag("player");
  rightPlayerObjWithCollider->setTag("player");


  std::string PlayerModelPath = "Models/Player/player_animations.fbx";
  std::string AnimatedEnemyPAth = "Models/" + ConfigUtils::GetValueFromMap<std::string>("Enemy_Animated_Model",  ConfigUtils::GlobalConfigMap);

  playerModel = new AnimatedModel(PlayerModelPath, *shaderAnimatedModel, false);
  player2Model = new AnimatedModel(*playerModel);
  enemyModel = new AnimatedModel(AnimatedEnemyPAth, *shaderAnimatedModel, false);

  ShapeRenderer3D *TileRenderer = new ShapeRenderer3D(Shapes::RainBow_Square,
                                                      Shapes::RB_Square_indices,
                                                      sizeof(Shapes::RainBow_Square),
                                                      sizeof(Shapes::RB_Square_indices),
                                                      *shaderProgram,
                                                      FreeTileTexture, "Basic");

  ConeRenderer *coneRendererLeft = new ConeRenderer(*shaderViewCone, &leftScene);

  leftPlayerObj->AddComponent(coneRendererLeft);
  leftPlayerObj->AddComponent(playerModel);
  rightPlayerObj->AddComponent(player2Model);
  enemyGameObject->AddComponent(enemyModel);
  

  Collider *leftPlayerCollider = new Collider(leftPlayerObjWithCollider->transform);
  Collider *rightPlayerCollider = new Collider(rightPlayerObjWithCollider->transform);

  EnemyKills* killer = new EnemyKills(Enemy_Node.local, &leftPlayerNode);
  enemyGameObject->AddComponent(killer);

  leftPlayerCollider->setDimensions(-0.12, 0, 0.25, 2.3/15, 2, 3.05/15);
  rightPlayerCollider->setDimensions(-0.12, 0, 0.25, 2.3/15, 2, 3.05/15);

  leftPlayerObjWithCollider->AddComponent(leftPlayerCollider);
  rightPlayerObjWithCollider->AddComponent(rightPlayerCollider);

  leftPlayerNode.AddGameObject(leftPlayerObjWithCollider);
  rightPlayerNode.AddGameObject(rightPlayerObjWithCollider);

  leftPlayerNodeForModel.AddGameObject(leftPlayerObj);
  rightPlayerNodeForModel.AddGameObject(rightPlayerObj);

  Enemy_Node_For_Model.AddGameObject(enemyGameObject);

  float floorTransform = ConfigUtils::GetValueFromMap<float>("FloorTranslation", ConfigUtils::GlobalConfigMap);
  float TileScale = ConfigUtils::GetValueFromMap<float>("TileScale", ConfigUtils::GlobalConfigMap);

  leftPlayerNode.Scale(PlayerScale);
  rightPlayerNode.Scale(PlayerScale);

  Enemy_Node.Scale(EnemyScale);

  leftPlayerNode.Translate(0, PlayerYOffset, 0);
  rightPlayerNode.Translate(0, PlayerYOffset, 0);
  playerObj = new Player(&leftPlayerNode, 3, *shaderProgram, &leftScene);


  leftPlayerNode.AddChild(&leftPlayerNodeForModel);
  rightPlayerNode.AddChild(&rightPlayerNodeForModel);

  Enemy_Node.AddChild(&Enemy_Node_For_Model);
  sNodes.push_back(&Enemy_Node);

  enemyController = new EnemyController(Enemy_Node,
										leftPlayerNode , 
										GridLocation{ static_cast<int>(Corners[0].x),static_cast<int>(Corners[0].y)}, //enemy start location
										GridLocation{ static_cast<int>(Corners[1].x),static_cast<int>(Corners[1].y) }, //enemy firstTarget
										grid,
										mapTiles,
									    MapSize); //reference for the map


  Enemy_Node.Translate(Corners[0].x * EnemyScaleInverse, EnemyYoffset * 100, Corners[0].y * EnemyScaleInverse);
  leftPlayerNode.Translate(Corners[3].x* PlayerScaleInverse, 0, Corners[3].y * PlayerScaleInverse);
  rightPlayerNode.Translate(Corners[3].x* PlayerScaleInverse, 0, Corners[3].y * PlayerScaleInverse);

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

  for (auto node : generator.leftnodes)
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
  gatherCollidableObjects(leftScene.children);
  std::cout<<"Colliders gathered: " << collidableObjects.size() << std::endl;
  gatherTriggers(sNodes);
  std::cout << "Triggers gathered: " << triggers.size() << std::endl;
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


  ImguiClear();
  glDeleteProgram(shaderProgram->shaderProgramID);
  glfwTerminate();
}
void Game::CheckPlayerDeath()
{
	//std::cout << "Monster position * scale: " << Enemy_Node.local.getPosition().x * Enemy_Node.local.getScale().x << ", " << Enemy_Node.local.getPosition().z * Enemy_Node.local.getScale().z << std::endl;
}
void Game::Update(float interpolation)
{
	CheckPlayerDeath();

  if (!inputBlockade)
  {
 
	enemyController->Update(interpolation);

    if (leftSideActive)
      UpdatePlayer(leftPlayerNode, camera, interpolation);
    else
      UpdatePlayer(rightPlayerNode, camera2, interpolation);
  }

  enemyModel->Update();
  playerModel->Update();
  player2Model->Update();

  if (leftSideActive)
  {
	  ProcessInput(interpolation, camera);
  }
  else
  {
	  ProcessInput(interpolation, camera2);
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

		SetViewAndPerspective(camera, leftPlayerNode.local, &Enemy_Node.local);
		// RENDER LEWEJ STRONY
		glViewport(0, 0, (Game::WINDOW_WIDTH / 2) + 125, Game::WINDOW_HEIGHT);
		glScissor(0, 0, (Game::WINDOW_WIDTH / 2) + offset, Game::WINDOW_HEIGHT);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		leftScene.Render(originTransform, true);
		Enemy_Node.Render(originTransform, true);

		SetViewAndPerspective(camera2, rightPlayerNode.local, nullptr);

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

  FixAnimation();
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


  if (leftSideActive)
  {
	  SetPlayerRotation(player, movementDir,playerModel);
  }
  else
  {
	  SetPlayerRotation(player, movementDir, player2Model);
  }
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
  auto coneRenderer = (ConeRenderer *)(player.children[0]->gameObject->GetComponent(ComponentSystem::ComponentType::ConeRenderer));
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

  playerObj->Update(&okienko, TrapScale);
}

void Game::gatherCollidableObjects(std::vector<SceneNode *> &nodes)
{
  for (auto node : nodes)
  {
    if (node->gameObject != nullptr)
    {
     // std::cout<<node->gameObject->getTag()<<"\n";

      if (node->gameObject->getTag() != "player" && node->gameObject->getTag() != "enemy" && node->gameObject->getTag() != "floor")
      {
        ComponentSystem::Component *possibleCollider = node->gameObject->GetComponent(ComponentSystem::ComponentType::Collider);
        if (possibleCollider != nullptr)
        {
          collidableObjects.push_back((Collider *)possibleCollider);
        }
        
      }
    
    }
      gatherCollidableObjects(node->children);
  }

}
void Game::gatherTriggers(std::vector<SceneNode *> &nodes)
{
  for (auto node : nodes)
  {
    if (node->gameObject != nullptr)
    {
		if (node->gameObject->getTag() != "player")// && node->gameObject->getTag() != "enemy")
		{
			ComponentSystem::Component *possibleTrigger = node->gameObject->GetComponent(ComponentSystem::ComponentType::Trigger);
			if (possibleTrigger != nullptr)
			{
				triggers.push_back((Trigger *)possibleTrigger);
			}
		}
    }
	gatherTriggers(node->children);
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

void Game::SetViewAndPerspective(Camera &aCamera, Transform &player, Transform *enemy)
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
  shaderProgram_For_Model->setVec3("viewPos", aCamera.Position);
  auto lightPos = player.getPosition()*player.getScale();
  lightPos.y = 0.5;
  shaderProgram_For_Model->setVec3("pointLights[0].position", lightPos);
  glm::vec3 enemyLightPos;
  if (enemy != nullptr)
  {
	  enemyLightPos = enemy->getPosition()*enemy->getScale();
	  enemyLightPos.y = 0.5;
  }
  else
  {
	  //we don't want this light to be visible
	  enemyLightPos.y = -1;
  }
  shaderProgram_For_Model->setVec3("pointLights[1].position", enemyLightPos);

  shaderViewCone->use();
  shaderViewCone->setMat4("projection", projection);
  shaderViewCone->setMat4("view", view);

  shaderAnimatedModel->use();
  shaderAnimatedModel->setMat4("projection", projection);
  shaderAnimatedModel->setMat4("view", view);
  //shaderAnimatedModel->setFloat("FogDensity", FogDensity);
  shaderAnimatedModel->setFloat("viewSpaceZOffset", cameraZOffset);
}

// Funkcje do wyswietlania grafik
void Game::Plot()
{
	if (imgMode == 0)
	{
		inputBlockade = false;
		return;
	}

	if (imgMode == 1)
	{
		static std::string path;
		inputBlockade = true;

		static int currentPlotImage = 1;
		static int existingPlotImage = 1;
		static int currentPlotNumber = 1;

		static bool keyPressed = false;
		if (glfwGetKey(okienko.window, GLFW_KEY_ENTER) == GLFW_PRESS && !keyPressed)
		{
			currentPlotImage++;
			keyPressed = true;
		}
		else if (!glfwGetKey(okienko.window, GLFW_KEY_ENTER) == GLFW_PRESS && keyPressed)
			keyPressed = false;

		if (currentPlotNumber != plotNumber)
		{
			currentPlotImage = 1;
			existingPlotImage = 0;
			currentPlotNumber = plotNumber;
		}

		path = "Textures/" + std::to_string(plotNumber) + "_" + std::to_string(currentPlotImage) +".png";

		static Texture *tex;
		if (currentPlotImage == existingPlotImage)
		{
			existingPlotImage++;
			if (FILE *file = fopen(path.c_str(), "r"))
			{
				std::cout << "OTWARTO PLIK Z GRAFIKA FABULARNA : " << path << std::endl;
				tex = new Texture(path.c_str(), GL_NEAREST_MIPMAP_NEAREST);
				tex->Load();
				fclose(file);
			} else {
				currentPlotImage = 1;
				existingPlotImage = 0;
				currentPlotNumber++;
				imgMode = 2;
			}
		}
		if (tex != nullptr)
		{
			DisplayImage(path.c_str(), "Napis", tex);
		}
	}

	if (imgMode == 2)
	{
		inputBlockade = false;
		switch (trapPieces)
		{
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		}
	}
}
void Game::DisplayImage(const char *path, const char *text, Texture *imageTex)
{
  glViewport(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glScissor(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
  glEnable(GL_SCISSOR_TEST);
  glClearColor(1, 0.5, 0.5, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  SceneNode *imageNode = new SceneNode();
  GameObject *imageObj = new GameObject(imageNode->world);

  ShapeRenderer3D *image = new ShapeRenderer3D(
      Shapes::RainBow_Square,
      Shapes::RB_Square_indices,
      sizeof(Shapes::RainBow_Square),
      sizeof(Shapes::RB_Square_indices),
      *shaderProgram,
      imageTex, "Image");

  imageObj->AddComponent(image);
  imageNode->AddGameObject(imageObj);

  imageNode->Translate(camera.Position.x, camera.Position.y - 0.184f, camera.Position.z - 0.1f);
  imageNode->Rotate(camera.Pitch, glm::vec3(1, 0, 0));
  imageNode->Scale(12.8f / 30.0f, 7.2f / 30.0f, 1);

  Transform origin = Transform::origin();
  imageNode->Render(origin, true);
  
  //delete imageTex;
  delete imageNode;
  delete imageObj;
  delete image;
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


    ImGui::Text("Vector to move = %f, %f", vector2DHelper.x, vector2DHelper.y);
    ImGui::Text("Enemy position = %f, %f", Enemy_Node.local.getPosition().x*EnemyScale, Enemy_Node.local.getPosition().z*EnemyScale);
    ImGui::Text("Player position = %f, %f", leftPlayerNode.local.getPosition().x*PlayerScale, leftPlayerNode.local.getPosition().z*PlayerScale);
    ImGui::SliderFloat("CameraZOffset", &cameraZOffset, 0, 5);
    ImGui::SliderFloat("CameraYOffset", &cameraYOffset, 0, 5);
    ImGui::SliderFloat("FogDensity", &FogDensity, 0, 1);
	ImGui::SliderFloat("EnemyInterestMeter", &enemyController->InterestMeter,0,2000 );
	ImGui::SliderFloat("EnemyPlayerDistance", &enemyController->EnemyPlayerDistance, 0, 5000);
    ImGui::SliderFloat("CameraYaw", &camera.Yaw, -180, 180);
    ImGui::SliderFloat("CameraPitch", &camera.Pitch, -180, 180);

    camera.updateCameraVectors();
    camera2.updateCameraVectors();

 
    if (ImGui::Button("List Model AnimationNames"))
    {
		if (enemyModel != nullptr && playerModel != nullptr )
		{
			DisplayAnimationInfo(enemyModel);
			DisplayAnimationInfo(playerModel);
		}
    }

    if (ImGui::Button("Swtich Enemy Animation"))
    {
      if (enemyModel != nullptr)
      {
        if (enemyModel->GetAnimationNR() == 1)
        {
          enemyModel->SelectAnimation(0);
        }
        else
        {
          enemyModel->SelectAnimation(1);
        }
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

void Game::SetPlayerRotation(SceneNode & aPlayer, glm::vec3 movementDir,AnimatedModel * aPlayerModel)
{

	if (movementDir.x == 1 || movementDir.z == 1 || movementDir.x == -1 || movementDir.z == -1)
	{
		aPlayerModel->SelectAnimation("Player_Walk");
	}
	else
	{
		aPlayerModel->SelectAnimation("player_idle");
	}

	if (movementDir.z == -1 && movementDir.x == 0)
		aPlayer.children[0]->local.SetRotation(0, 180, 0);
	else if (movementDir.z == 1 && movementDir.x == 0)
		aPlayer.children[0]->local.SetRotation(0, 0, 0);
	else if (movementDir.x == -1 && movementDir.z == 0)
		aPlayer.children[0]->local.SetRotation(0, 270, 0);
	else if (movementDir.x == 1 && movementDir.z == 0)
		aPlayer.children[0]->local.SetRotation(0, 90, 0);
	else if (movementDir.x == 1 && movementDir.z == -1)
		aPlayer.children[0]->local.SetRotation(0, 135, 0);
	else if (movementDir.x == -1 && movementDir.z == -1)
		aPlayer.children[0]->local.SetRotation(0, 225, 0);
	else if (movementDir.x == -1 && movementDir.z == 1)
		aPlayer.children[0]->local.SetRotation(0, 315, 0);
	else if (movementDir.x == 1 && movementDir.z == 1)
		aPlayer.children[0]->local.SetRotation(0, 45, 0);

}
void Game::FixAnimation()
{
	if (leftSideActive)
	{
		player2Model->SelectAnimation("player_idle");
	}
	else
	{
		playerModel->SelectAnimation("player_idle");
	}
}

void Game::SetupPlayersColiders()
{
}