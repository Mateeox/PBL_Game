#pragma once

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Include GL3W (OpenGL function loader)
#include <GL/gl3w.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include <stdexcept>

#include "Camera.hpp"


namespace PBLGame{

static bool mouseToUpdate = false;
static double posx;
static double posy;

class Window
{

  bool mouseCallBack = true;
  bool firstMouse = true;
  double lastY;
  double lastX;
  uint16_t iWidth;
  uint16_t iHeight;

public:
  void SetCamera(Camera camera);
  void ProcessMouse();
  void ProcessInput(float interpolation);

  Camera camera;
  GLFWwindow *window;
  int Initialise();
  Window(uint16_t Width, uint16_t height, const char *title);
};
}