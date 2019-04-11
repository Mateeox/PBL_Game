#pragma once
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
  SceneNode scena3_new;

  GameObject *trojObj = new GameObject(scena1_new.world);
  GameObject *FloorObj = new GameObject(FloorNode_new.world);
  GameObject *hexObj = new GameObject(scena3_new.world);

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
  FloorNode_new.Rotate(90.0f, glm::vec3(1, 0, 0));
  FloorNode_new.Scale(100, 100, 100);

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



  delete trojObj;
  delete FloorObj;
  delete hexObj;

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

  projection = glm::perspective(okienko.camera.Zoom, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
  view = okienko.camera.GetViewMatrix();
  shaderProgram->setMat4("projection", projection);
  shaderProgram->setMat4("view", view);

  glfwPollEvents();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();


  /*if (show_demo_window)
  {
    ImGui::Begin("Another Window",
                 &show_demo_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
    ImGui::Text("Hello from another window!");
    if (ImGui::Button("Close Me"))
    {
      show_demo_window = false;
    }
    ImGui::End();
  }*/
  ImGui::Render();


  Transform originTransform = Transform::origin();

  // Render lewej kamery
  glViewport(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

  for (auto node : sNodes)
  {
    node.Render(originTransform, true);
  }

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


  // Render prawej kamery
  glViewport(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

  /*for (auto node : sNodes)
  {
	  node.Render(originTransform, true);
  }*/

  sNodes[1].Render(originTransform, true);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  // Swap buffers
  glfwSwapBuffers(okienko.window);
}