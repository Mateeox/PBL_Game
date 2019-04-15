#pragma once
#include "Window.hpp"
#include "Shader.hpp"
#include "SceneNode.hpp"
#include <map>

const int TICKS_PER_SECOND = 128;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

using namespace PBLGame;

class Game{

    Window& okienko;
    Shader *shaderProgram;
    Shader *shaderProgram_For_Model;     
    std::vector<SceneNode> sNodes;

    //camera
    glm::mat4 view;
    glm::mat4 projection;

    //Imgui
    bool show_demo_window = true;
	int offset = 250;	// Jak bardzo maja sie roznic rozmiary kamery, szerokosc aktywnej to pol okna + offset, szerokosc nieaktywnej to pol okna - offset

    public:
	static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;
    Game(Window& okienko);

    
    void Granko();
    void Update(float interpolation);
    void Render();
	void Serialize();
	void Deserialize(std::string path);
	
	private:
	void SerializeFaza1(std::map <SceneNode*, unsigned> &map);
	void SerializeFaza2(std::map <SceneNode*, unsigned> &map, std::vector<SceneNode> &temp);
	void SerializeFaza3(std::vector<SceneNode> &temp);
	void SerializeZapisz(std::string serialized);
	void DeserializeOrderPointers(std::map<unsigned, SceneNode*> &map);
};