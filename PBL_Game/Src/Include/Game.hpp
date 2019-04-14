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
    std::vector<SceneNode> sNodes;

    //camera
    glm::mat4 view;
    glm::mat4 projection;

    //Imgui
    bool show_demo_window = true;

    public:
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