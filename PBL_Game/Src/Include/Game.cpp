#include "Game.hpp"
#include "ShapeRenderer3D.hpp"
#include "Shapes.hpp"


Game::Game(Window &aOkno) : okienko(aOkno)
{
  shaderProgram = new Shader("Shaders/vertex4.txt", "Shaders/fragment3.txt");
}

void Game::Granko()
{

  Texture *xD = new Texture("Textures/red.png", GL_LINEAR);

  SceneNode scena1_new;
  SceneNode FloorNode_new;
  SceneNode box1;
  SceneNode box2;
  SceneNode box3;

  GameObject *trojObj = new GameObject(scena1_new.world);
  GameObject *FloorObj = new GameObject(FloorNode_new.world);
  GameObject *hexObj1 = new GameObject(box1.local);
  hexObj1->setTag("player");
  GameObject *hexObj2 = new GameObject(box2.local);
  GameObject *hexObj3 = new GameObject(box3.local);


  ShapeRenderer3D *Floor = new ShapeRenderer3D(Shapes::RainBow_Square,
                                               Shapes::RB_Square_indices,
                                               sizeof(Shapes::RainBow_Square),
                                               sizeof(Shapes::RB_Square_indices),
                                               *shaderProgram,
                                               nullptr);

  ShapeRenderer3D *trojkat = new ShapeRenderer3D(Shapes::RainBow_Triangle,
                                                 Shapes::RB_Triangle_indices,
                                                 sizeof(Shapes::RainBow_Triangle),
                                                 sizeof(Shapes::RB_Triangle_indices),
                                                 *shaderProgram,
                                                 nullptr);

  ShapeRenderer3D *szescian = new ShapeRenderer3D(Shapes::RainBow_Cube,
                                                  Shapes::RB_Cube_indices,
                                                  sizeof(Shapes::RainBow_Cube),
                                                  sizeof(Shapes::RB_Cube_indices),
                                                  *shaderProgram,
                                                  xD);

  trojObj->AddComponent(trojkat);
  FloorObj->AddComponent(Floor);
  hexObj1->AddComponent(szescian);
  hexObj2->AddComponent(szescian);
  hexObj3->AddComponent(szescian);

  Collider* box1Collider = new Collider(hexObj1->transform);
  box1Collider->setDimensions(0, 0, 0, 0.2, 1.2, 2);
  hexObj1->AddComponent(box1Collider);
  Collider* box2Collider = new Collider(hexObj2->transform);
  box2Collider->setDimensions(0, 0, 0, 2, 2, 2);
  hexObj2->AddComponent(box2Collider);
  Collider* box3Collider = new Collider(hexObj3->transform);
  box3Collider->setDimensions(0, 0, 0, 2, 2, 2);
  hexObj3->AddComponent(box3Collider);

  scena1_new.AddGameObject(trojObj);
  FloorNode_new.AddGameObject(FloorObj);
  box1.AddGameObject(hexObj1);
  box2.AddGameObject(hexObj2);
  box3.AddGameObject(hexObj3);

 // box1.Scale(0.1f, 1.0f, 1.0f);
  box1.Scale(0.1f, 0.6f, 1.0f);
  box2.Translate(3, 0, 0);
  box3.Translate(-2, 0, 0);
  FloorNode_new.Translate(0.0f, -1.0f, 0.1f);
  FloorNode_new.Rotate(90.0f, glm::vec3(1, 0, 0));
  FloorNode_new.Scale(100, 100, 100);

  sNodes.push_back(&scena1_new);
  sNodes.push_back(&FloorNode_new);
  sNodes.push_back(&box1);
  sNodes.push_back(&box2);
  sNodes.push_back(&box3);

  shaderProgram->use();

  uint32 next_game_tick = (glfwGetTime() * 1000);
  int loops;
  float interpolation = 1.0;
  short direction = 1;
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

	//this "direction" thing will soon be deleted...
	if (glfwGetKey(okienko.window, GLFW_KEY_P) == GLFW_PRESS)
	{
		direction = 1;
	}
	if (glfwGetKey(okienko.window, GLFW_KEY_L) == GLFW_PRESS)
	{
		direction = -1;
	}
	UpdatePlayer(box1, direction);
	

    interpolation =
        float((glfwGetTime() * 1000) + SKIP_TICKS - next_game_tick) /
        float(SKIP_TICKS);

    Render();
  }



  delete trojObj;
  delete FloorObj;
  delete hexObj1;
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
  okienko.ProcessInput(interpolation);
}

void Game::Render()
{

  projection = glm::perspective(okienko.camera.Zoom, 1280.0f / 720.0f, 0.1f, 100.0f);
  view = okienko.camera.GetViewMatrix();
  shaderProgram->setMat4("projection", projection);
  shaderProgram->setMat4("view", view);

  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (show_demo_window)
  {
    ImGui::Begin("Another Window",
                 &show_demo_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
    {
      show_demo_window = false;
    }
    ImGui::End();
  }
  ImGui::Render();

  Transform originTransform = Transform::origin();

  for (auto node : sNodes)
  {
    node->Render(originTransform, true);
  }

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers
  glfwSwapBuffers(okienko.window);
}

void Game::UpdatePlayer(SceneNode& player, short direction)
{
	Transform transformBeforeMove(player.gameObject->transform);
	player.Translate(0.01 * direction, 0, 0);
	bool coll = false;
	Collider* playerCollider = ((Collider*)player.gameObject->GetComponent(ComponentSystem::ComponentType::Collider));
	for (Collider* collider : collidableObjects)
	{
		if (playerCollider->checkCollision(collider))
		{
			player.local = transformBeforeMove;
			playerCollider->transform = transformBeforeMove;
			coll = true;
			break;
		}
	}
	//Collider* tmp = ((Collider*)box2.gameObject->GetComponent(ComponentSystem::ComponentType::Collider));
	

	coll ? printf("true\n") : printf("false\n");
}

void Game::gatherCollidableObjects(std::vector<SceneNode*>& nodes)
{
	for (auto node : nodes)
	{
		if (node->gameObject->getTag() != "player" && node->gameObject->getTag() != "enemy")
		{
			ComponentSystem::Component* possibleCollider = node->gameObject->GetComponent(ComponentSystem::ComponentType::Collider);
			if (possibleCollider != nullptr)
			{
				collidableObjects.push_back((Collider*)possibleCollider);
			}
			gatherCollidableObjects(node->children);
		}
	}
}

