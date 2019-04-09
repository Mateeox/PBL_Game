
#include "Window.hpp"
#include "Camera.hpp"

namespace PBLGame{

static bool First_tab_push = false;
static bool Tab_Pressed = false;

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

Window::Window(uint16_t Width, uint16_t height, const char *title) : iWidth(Width), iHeight(height), camera(Camera())
{

	// Initialise GLFW
	if (!glfwInit())
	{
		printf("glfwInit Fail !!! \n");
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(Width, height, title, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		printf("Failed to open Window \n");
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	bool gl3w_err = gl3wInit() != 0;
	if (gl3w_err)
	{
		fprintf(stderr, "Failed to initialize GL3W\n");
		glfwTerminate();
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//setup Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	const char *glsl_version = "#version 330";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
}

void Window::SetCamera(Camera aCamera)
{
	this->camera = aCamera;
}
void Window::ProcessInput(float interpolation)
{

	ProcessMouse();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, interpolation);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, interpolation);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, interpolation);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, interpolation);

	if (!Tab_Pressed)
	{
		if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		{
			Tab_Pressed = true;

			Cursor_Enabled = !Cursor_Enabled;
			if (Cursor_Enabled)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				glfwSetCursorPos(window, iWidth / 2, iHeight / 2);
			}
			else
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
	{
		Tab_Pressed = false;
	}
}
void Window::ProcessMouse()
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

}