#pragma once
#include "Window.hpp"
#include "Shader.hpp"
#include "SceneNode.hpp"
#include "Collider.hpp"
#include <map>

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
    std::vector<SceneNode*> sNodes;
    std::vector<Collider*> collidableObjects;

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
    int offset = 125; // Jak bardzo maja sie roznic rozmiary kamery, szerokosc aktywnej to pol okna + offset, szerokosc nieaktywnej to pol okna - offset

  public:
    static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
    Game(Window &okienko);

    void SetCamera(Camera camera, int camera_nr);
    void ProcessMouse();
    void ProcessInput(float interpolation, Camera &camera_update);

    void Granko();
    void Update(float interpolation);
    void Render();
    void Serialize();
    void Deserialize(std::string path);
	void UpdatePlayer(SceneNode& player);
	void gatherCollidableObjects(std::vector<SceneNode*>& nodes);

  private:
    void SerializeFaza1(std::map<SceneNode *, unsigned> &map);
    void SerializeFaza2(std::map<SceneNode *, unsigned> &map, std::vector<SceneNode> &temp);
    void SerializeFaza3(std::vector<SceneNode> &temp);
    void SerializeZapisz(std::string serialized);
    void DeserializeOrderPointers(std::map<unsigned, SceneNode *> &map);

    void SetViewAndPerspective(Camera &aCamera);
};