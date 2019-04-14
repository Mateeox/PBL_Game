#include "Game.hpp"
#include "ShapeRenderer3D.hpp"
#include "Shapes.hpp"

Game::Game(Window &aOkno) : okienko(aOkno)
{
  shaderProgram = new Shader("Shaders/vertex4.txt", "Shaders/fragment3.txt");
}

void Game::Granko()
{

  Texture * xD = new Texture("Textures/red.png",GL_LINEAR);


SceneNode scena1_new;
  SceneNode FloorNode_new;
  SceneNode scena3_new;

  GameObject * trojObj = new GameObject(scena1_new.world);
   GameObject * FloorObj = new GameObject(FloorNode_new.world);
    GameObject * hexObj = new GameObject(scena3_new.world);

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
  hexObj->AddComponent(szescian);
  

  scena1_new.AddGameObject(trojObj);
  FloorNode_new.AddGameObject(FloorObj);
  scena3_new.AddGameObject(hexObj);

  scena3_new.Scale(0.3f, 0.2f, 1.0f);
  FloorNode_new.Translate(0.0f, -1.0f, 0.1f);
  FloorNode_new.Rotate(90.0f,glm::vec3(1,0,0));
  FloorNode_new.Scale(100,100,100);

  sNodes.push_back(scena1_new);
  sNodes.push_back(FloorNode_new);
  sNodes.push_back(scena3_new);


  shaderProgram->use();

  uint32 next_game_tick = (glfwGetTime() * 1000);
  int loops;
  float interpolation = 1.0;

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
   Deserialize("output.txt");
   Serialize();
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
 

  for (auto node : sNodes) {
     node.Render(originTransform, true);
  }

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers
  glfwSwapBuffers(okienko.window);
}

void Game::Serialize()
{
	std::map<SceneNode*, unsigned> oidMap;
	SerializeFaza1(oidMap);
	std::vector<SceneNode> tempNodes;
	SerializeFaza2(oidMap, tempNodes);
	oidMap.clear();
	SerializeFaza3(tempNodes);
	tempNodes.clear();
}

void Game::SerializeFaza1(std::map<SceneNode*, unsigned> &map)
{
	this->sNodes[0].AddChild(&this->sNodes[1]);
	this->sNodes[1].AddParent(&this->sNodes[0]);
	for (SceneNode &scene : this->sNodes)
	{
		unsigned n = map.size() + 1;
		map.insert(std::pair<SceneNode*, unsigned>(&scene, n));
	}
}

void Game::SerializeFaza2(std::map<SceneNode*, unsigned> &map, std::vector<SceneNode> &temp)
{
	for(std::pair<SceneNode*, unsigned> scene : map)
	{
		SceneNode tempNode;
		tempNode.AddParent((SceneNode *)map[scene.first->parent]);

		if(scene.first->children.size() > 0) 
			for (SceneNode* child : scene.first->children)
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
	std::ofstream out("output.txt");
	out << serialized;
	out.close();
}

void Game::Deserialize(std::string path)
{
	std::ifstream in(path);
	std::map<unsigned, SceneNode*> oidMap;
	unsigned index = 1;
	std::string line;
	while (std::getline(in, line))
	{
		if (line.substr(0, 2) == "SN")
		{
			SceneNode node;
			//this->sNodes.push_back(node);  TEMPORARY
			oidMap.insert(std::pair<unsigned, SceneNode*>(index, &node));
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
			unsigned i_dec = atoi(line.substr(3).c_str());
			oidMap[index - 1]->AddParent((SceneNode*)i_dec);
		}

		if (line.substr(0, 4) == "\tCH;")
		{
			unsigned i_dec = atoi(line.substr(4).c_str());
			oidMap[index - 1]->AddChild((SceneNode*)i_dec);
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
}