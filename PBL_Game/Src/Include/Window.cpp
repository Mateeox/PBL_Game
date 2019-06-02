
#include "Window.hpp"

namespace PBLGame{


Window::Window(uint16_t Width, uint16_t height, const char *title) : iWidth(Width), iHeight(height)
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

}